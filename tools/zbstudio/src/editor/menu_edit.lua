-- Copyright 2011-16 Paul Kulchenko, ZeroBrane LLC
-- authors: Lomtik Software (J. Winwood & John Labenski)
-- Luxinia Dev (Eike Decker & Christoph Kubisch)
---------------------------------------------------------

local ide = ide

-- ---------------------------------------------------------------------------
-- Create the Edit menu and attach the callback functions

local frame = ide.frame
local menuBar = frame.menuBar

local editMenu = ide:MakeMenu {
  { ID_UNDO, TR("&Undo")..KSC(ID_UNDO), TR("Undo last edit") },
  { ID_REDO, TR("&Redo")..KSC(ID_REDO), TR("Redo last edit undone") },
  { },
  { ID_CUT, TR("Cu&t")..KSC(ID_CUT), TR("Cut selected text to clipboard") },
  { ID_COPY, TR("&Copy")..KSC(ID_COPY), TR("Copy selected text to clipboard") },
  { ID_PASTE, TR("&Paste")..KSC(ID_PASTE), TR("Paste text from the clipboard") },
  { ID_SELECTALL, TR("Select &All")..KSC(ID_SELECTALL), TR("Select all text in the editor") },
  { },
  { ID_SHOWTOOLTIP, TR("Show &Tooltip")..KSC(ID_SHOWTOOLTIP), TR("Show tooltip for current position; place cursor after opening bracket of function") },
  { ID_AUTOCOMPLETE, TR("Complete &Identifier")..KSC(ID_AUTOCOMPLETE), TR("Complete the current identifier") },
  { ID_AUTOCOMPLETEENABLE, TR("Auto Complete Identifiers")..KSC(ID_AUTOCOMPLETEENABLE), TR("Auto complete while typing"), wx.wxITEM_CHECK },
  { },
  { ID_SOURCE, TR("Source"), "", {
    { ID_COMMENT, TR("C&omment/Uncomment")..KSC(ID_COMMENT), TR("Comment or uncomment current or selected lines") },
    { ID_REINDENT, TR("Correct &Indentation")..KSC(ID_REINDENT), TR("Re-indent selected lines") },
    { ID_FOLD, TR("&Fold/Unfold All")..KSC(ID_FOLD), TR("Fold or unfold all code folds") },
    { ID_FOLDLINE, TR("Fold/Unfold Current &Line")..KSC(ID_FOLDLINE), TR("Fold or unfold current line") },
    { ID_SORT, TR("&Sort")..KSC(ID_SORT), TR("Sort selected lines") },
  } },
  { ID_BOOKMARK, TR("Bookmark"), "", {
    { ID_BOOKMARKTOGGLE, TR("Toggle Bookmark")..KSC(ID_BOOKMARKTOGGLE), TR("Toggle bookmark") },
    { ID_BOOKMARKNEXT, TR("Go To Next Bookmark")..KSC(ID_BOOKMARKNEXT) },
    { ID_BOOKMARKPREV, TR("Go To Previous Bookmark")..KSC(ID_BOOKMARKPREV) },
  } },
  { },
  { ID_PREFERENCES, TR("Preferences"), "", {
    { ID_PREFERENCESSYSTEM, TR("Settings: System")..KSC(ID_PREFERENCESSYSTEM) },
    { ID_PREFERENCESUSER, TR("Settings: User")..KSC(ID_PREFERENCESUSER) },
  } },
}
menuBar:Append(editMenu, TR("&Edit"))

editMenu:Check(ID_AUTOCOMPLETEENABLE, ide.config.autocomplete)

local function getCtrlWithFocus(edType)
  local ctrl = ide:GetMainFrame():FindFocus()
  return ctrl and ctrl:GetClassInfo():GetClassName() == edType and ctrl:DynamicCast(edType) or nil
end

local function onUpdateUIEditorInFocus(event)
  event:Enable(ide:GetEditorWithFocus(ide:GetEditor()) ~= nil)
end

local function onUpdateUIEditMenu(event)
  local menu_id = event:GetId()
  local editor = ide:GetEditorWithFocus()
  if editor == nil then
    local editor = getCtrlWithFocus("wxTextCtrl")
    event:Enable(editor and (
        menu_id == ID_PASTE and editor:CanPaste() or
        menu_id == ID_UNDO and editor:CanUndo() or
        menu_id == ID_REDO and editor:CanRedo() or
        menu_id == ID_CUT and editor:CanCut() or
        menu_id == ID_COPY and editor:CanCopy() or
        menu_id == ID_SELECTALL and true
      ) or false)
    return
  end

  local alwaysOn = {
    [ID_SELECTALL] = true,
    -- allow Cut and Copy commands as these work on a line if no selection
    [ID_COPY] = true, [ID_CUT] = true,
  }
  local enable =
    -- pasting is allowed when the document is not read-only and the selection
    -- (if any) has no protected text; since pasting handles protected text,
    -- use GetReadOnly() instead of CanPaste()
    menu_id == ID_PASTE and (not editor:GetReadOnly()) or
    menu_id == ID_UNDO and editor:CanUndo() or
    menu_id == ID_REDO and editor:CanRedo() or
    alwaysOn[menu_id]
  event:Enable(enable)
end

local function onEditMenu(event)
  local menu_id = event:GetId()
  local editor = ide:GetEditorWithFocus()
  if editor == nil then
    local editor = getCtrlWithFocus("wxTextCtrl")
    if not editor or not (
      menu_id == ID_PASTE and editor:Paste() or
      menu_id == ID_UNDO and editor:Undo() or
      menu_id == ID_REDO and editor:Redo() or
      menu_id == ID_CUT and editor:Cut() or
      menu_id == ID_COPY and editor:Copy() or
      menu_id == ID_SELECTALL and editor:SetSelection(-1, -1) or
      true
    ) then event:Skip() end
    return
  end

  if PackageEventHandle("onEditorAction", editor, event) == false then
    return
  end

  local copytext
  if (menu_id == ID_CUT or menu_id == ID_COPY)
  and ide.wxver >= "2.9.5" and editor:GetSelections() > 1 then
    local main = editor:GetMainSelection()
    copytext = editor:GetTextRangeDyn(editor:GetSelectionNStart(main), editor:GetSelectionNEnd(main))
    for s = 0, editor:GetSelections()-1 do
      if copytext ~= editor:GetTextRangeDyn(editor:GetSelectionNStart(s), editor:GetSelectionNEnd(s)) then
        copytext = nil
        break
      end
    end
  end

  local spos, epos = editor:GetSelectionStart(), editor:GetSelectionEnd()
  if menu_id == ID_CUT then
    if spos == epos then
      if ide.config.editor.linecopy then editor:LineCopy() end
    else
      editor:CopyDyn()
    end
    if spos == epos and ide.config.editor.linecopy then
      local line = editor:LineFromPosition(spos)
      spos, epos = editor:PositionFromLine(line), editor:PositionFromLine(line+1)
      editor:SetSelectionStart(spos)
      editor:SetSelectionEnd(epos)
    end
    if spos ~= epos then editor:ClearAny() end
  elseif menu_id == ID_COPY then
    if spos == epos then
      if ide.config.editor.linecopy then editor:LineCopy() end
    else
      editor:CopyDyn()
    end
  elseif menu_id == ID_PASTE then
    -- first clear the text in case there is any hidden markup
    if spos ~= epos then editor:ClearAny() end
    editor:PasteDyn()
  elseif menu_id == ID_SELECTALL then editor:SelectAll()
  elseif menu_id == ID_UNDO then editor:Undo()
  elseif menu_id == ID_REDO then editor:Redo()
  end

  if copytext then editor:CopyText(#copytext, copytext) end
end

for _, event in pairs({ID_CUT, ID_COPY, ID_PASTE, ID_SELECTALL, ID_UNDO, ID_REDO}) do
  frame:Connect(event, wx.wxEVT_COMMAND_MENU_SELECTED, onEditMenu)
  frame:Connect(event, wx.wxEVT_UPDATE_UI, onUpdateUIEditMenu)
end

for _, event in pairs({
    ID_BOOKMARKTOGGLE, ID_BOOKMARKNEXT, ID_BOOKMARKPREV,
    ID_AUTOCOMPLETE, ID_SORT, ID_REINDENT, ID_SHOWTOOLTIP,
}) do
  frame:Connect(event, wx.wxEVT_UPDATE_UI, onUpdateUIEditorInFocus)
end

frame:Connect(ID_COMMENT, wx.wxEVT_UPDATE_UI,
  function(event)
    local editor = ide:GetEditorWithFocus(ide:GetEditor())
    event:Enable(editor ~= nil
      and ide:IsValidProperty(editor, "spec") and editor.spec
      and editor.spec.linecomment and true or false)
  end)

local function generateConfigMessage(type)
  return ([==[--[[--
  Use this file to specify **%s** preferences.
  Review [examples](+%s) or check [online documentation](%s) for details.
--]]--
]==])
    :format(type, MergeFullPath(ide.editorFilename, "../cfg/user-sample.lua"),
      "http://studio.zerobrane.com/documentation.html")
end

frame:Connect(ID_PREFERENCESSYSTEM, wx.wxEVT_COMMAND_MENU_SELECTED,
  function ()
    local editor = LoadFile(ide.configs.system)
    if editor and editor:GetLength() == 0 then
      editor:AddTextDyn(generateConfigMessage("System")) end
  end)

frame:Connect(ID_PREFERENCESUSER, wx.wxEVT_COMMAND_MENU_SELECTED,
  function ()
    local editor = LoadFile(ide.configs.user)
    if editor and editor:GetLength() == 0 then
      editor:AddTextDyn(generateConfigMessage("User")) end
  end)
frame:Connect(ID_PREFERENCESUSER, wx.wxEVT_UPDATE_UI,
  function (event) event:Enable(ide.configs.user ~= nil) end)

frame:Connect(ID_CLEARDYNAMICWORDS, wx.wxEVT_COMMAND_MENU_SELECTED,
  function () DynamicWordsReset() end)

frame:Connect(ID_SHOWTOOLTIP, wx.wxEVT_COMMAND_MENU_SELECTED,
  function (event)
    local editor = ide:GetEditor()

    if (editor:CallTipActive()) then
      editor:CallTipCancel()
      return
    end

    EditorCallTip(editor, editor:GetCurrentPos())
  end)

frame:Connect(ID_AUTOCOMPLETE, wx.wxEVT_COMMAND_MENU_SELECTED,
  function (event) EditorAutoComplete(ide:GetEditor()) end)

frame:Connect(ID_AUTOCOMPLETEENABLE, wx.wxEVT_COMMAND_MENU_SELECTED,
  function (event) ide.config.autocomplete = event:IsChecked() end)

frame:Connect(ID_COMMENT, wx.wxEVT_COMMAND_MENU_SELECTED,
  function (event)
    local editor = ide:GetEditor()
    local lc = editor.spec.linecomment
    if not lc then return end

    -- for multi-line selection, always start the first line at the beginning
    local ssel, esel = editor:GetSelectionStart(), editor:GetSelectionEnd()
    local sline = editor:LineFromPosition(ssel)
    local eline = editor:LineFromPosition(esel)
    local sel = ssel ~= esel
    local rect = editor:SelectionIsRectangle()
    local qlc = lc:gsub(".", "%%%1")

    -- figure out how to toggle comments; if there is at least one non-empty
    -- line that doesn't start with a comment, need to comment
    local comment = false
    for line = sline, eline do
      local pos = sel and (sline == eline or rect)
        and ssel-editor:PositionFromLine(sline)+1 or 1
      local text = editor:GetLineDyn(line)
      local _, cpos = text:find("^%s*"..qlc, pos)
      if not cpos and text:find("%S")
      -- ignore last line when the end of selection is at the first position
      and (line == sline or line < eline or esel-editor:PositionFromLine(line) > 0) then
        comment = true
        break
      end
    end

    local linetoggle = ide.config.editor.commentlinetoggle
    editor:BeginUndoAction()
    -- go last to first as selection positions we captured may be affected
    -- by text changes
    for line = eline, sline, -1 do
      local pos = sel and (sline == eline or rect) and ssel-editor:PositionFromLine(sline)+1 or 1
      local text = editor:GetLineDyn(line)
      local validline = (line == sline or line < eline or esel-editor:PositionFromLine(line) > 0)
      local _, cpos = text:find("^%s*"..qlc, pos)
      if (linetoggle or not comment) and cpos and validline then
        editor:DeleteRange(cpos-#lc+editor:PositionFromLine(line), #lc)
      elseif (linetoggle or comment) and text:find("%S") and validline then
        editor:SetTargetStart(pos+editor:PositionFromLine(line)-1)
        editor:SetTargetEnd(editor:GetTargetStart())
        editor:ReplaceTarget(lc)
      end
    end
    editor:EndUndoAction()
  end)

local function processSelection(editor, func)
  local text = editor:GetSelectedTextDyn()
  local line = editor:GetCurrentLine()
  local posinline = editor:GetCurrentPos() - editor:PositionFromLine(line)
  if #text == 0 then
    editor:SelectAll()
    text = editor:GetSelectedTextDyn()
  end
  local wholeline = text:find("\n$")
  local buf = {}
  for ln in string.gmatch(text..(wholeline and "" or "\n"), "(.-\r?\n)") do
    table.insert(buf, ln)
  end
  if #buf > 0 then
    if func then func(buf) end
    -- add new line at the end if it was there
    local newtext = table.concat(buf, ""):gsub("(\r?\n)$", wholeline and "%1" or "")
    -- straightforward editor:ReplaceSelection() doesn't work reliably as
    -- it sometimes doubles the context when the entire file is selected.
    -- this seems like Scintilla issue, so use ReplaceTarget instead.
    -- Since this doesn't work with rectangular selection, which
    -- ReplaceSelection should handle (after wxwidgets 3.x upgrade), this
    -- will need to be revisited when ReplaceSelection is updated.
    if newtext ~= text then
      editor:BeginUndoAction()
      -- if there is at least one marker, then use a different mechanism to preserve them
      -- simply saving markers, replacing text, and reapplying markers doesn't work as
      -- they get reset after `undo/redo` operations.
      local ssel, esel = editor:GetSelectionStart(), editor:GetSelectionEnd()
      local sline = editor:LineFromPosition(ssel)
      local eline = editor:LineFromPosition(esel)
      if #editor:MarkerGetAll(nil, sline, eline) > 0 then
        for line = #buf, 1, -1 do
          editor:SetTargetStart(line == 1 and ssel or editor:PositionFromLine(sline+line-1))
          editor:SetTargetEnd(line == eline-sline+1 and esel or editor:GetLineEndPosition(sline+line-1))
          editor:ReplaceTargetDyn((buf[line]:gsub("\r?\n$", "")))
        end
      else
        editor:TargetFromSelection()
        editor:ReplaceTargetDyn(newtext)
      end
      editor:EndUndoAction()
    end
  end
  editor:GotoPosEnforcePolicy(math.min(
      editor:PositionFromLine(line)+posinline, editor:GetLineEndPosition(line)))
end

frame:Connect(ID_SORT, wx.wxEVT_COMMAND_MENU_SELECTED,
  function (event) processSelection(ide:GetEditor(), table.sort) end)

local function reIndent(editor, buf)
  local decindent, incindent = editor.spec.isdecindent, editor.spec.isincindent
  if not (decindent and incindent) then return end

  local edline = editor:LineFromPosition(editor:GetSelectionStart())
  local indent = 0
  local text = ""
  -- find the last non-empty line in the previous block (if any)
  for n = edline-1, 1, -1 do
    indent = editor:GetLineIndentation(n)
    text = editor:GetLineDyn(n)
    if text:match("[^\r\n]") then break end
  end

  local ut = editor:GetUseTabs()
  local tw = ut and editor:GetTabWidth() or editor:GetIndent()

  local indents = {}
  local isstatic = {}
  for line = 1, #buf+1 do
    local ls = editor:PositionFromLine(edline+line-1)
    local style = bit.band(editor:GetStyleAt(ls), ide.STYLEMASK)
    -- don't reformat multi-line comments or strings
    isstatic[line] = (editor.spec.iscomment[style]
      or editor.spec.isstring[style]
      or (MarkupIsAny and MarkupIsAny(style)))
    if not isstatic[line] or line == 1 or not isstatic[line-1] then
      local closed, blockend = decindent(text)
      local opened = incindent(text)

      -- ignore impact from initial block endings as they are already indented
      if line == 1 then blockend = 0 end

      -- this only needs to be done for 2, #buf+1; do it and get out when done
      if line > 1 then indents[line-1] = indents[line-1] - tw * closed end
      if line > #buf then break end

      indent = indent + tw * (opened - blockend)
      if indent < 0 then indent = 0 end
    end

    indents[line] = indent
    text = buf[line]
  end

  for line = 1, #buf do
    if not isstatic[line] then
      buf[line] = buf[line]:gsub("^[ \t]*",
        not buf[line]:match("%S") and ""
        or ut and ("\t"):rep(indents[line] / tw) or (" "):rep(indents[line]))
    end
  end
end

frame:Connect(ID_REINDENT, wx.wxEVT_COMMAND_MENU_SELECTED,
  function (event)
    local editor = ide:GetEditor()
    processSelection(editor, function(buf) reIndent(editor, buf) end)
  end)

local function canfold(event)
  local editor = ide:GetEditorWithFocus()
  event:Enable(editor and editor:CanFold() or false)
end

frame:Connect(ID_FOLD, wx.wxEVT_UPDATE_UI, canfold)
frame:Connect(ID_FOLD, wx.wxEVT_COMMAND_MENU_SELECTED,
  function (event) ide:GetEditorWithFocus():FoldSome() end)

frame:Connect(ID_FOLDLINE, wx.wxEVT_UPDATE_UI, canfold)
frame:Connect(ID_FOLDLINE, wx.wxEVT_COMMAND_MENU_SELECTED,
  function (event)
    local editor = ide:GetEditorWithFocus()
    local current = editor:GetCurrentLine()
    editor:ToggleFold(current)
    -- move up to the parent line if the current one is not visible
    local visible = editor:GetLineVisible(current)
    if not visible and editor:GetFoldParent(current) ~= wx.wxNOT_FOUND then editor:LineUp() end
  end)

local BOOKMARK_MARKER = StylesGetMarker("bookmark")

frame:Connect(ID_BOOKMARKTOGGLE, wx.wxEVT_COMMAND_MENU_SELECTED,
  function() ide:GetEditor():BookmarkToggle() end)
frame:Connect(ID_BOOKMARKNEXT, wx.wxEVT_COMMAND_MENU_SELECTED,
  function() ide:GetEditor():MarkerGotoNext(BOOKMARK_MARKER) end)
frame:Connect(ID_BOOKMARKPREV, wx.wxEVT_COMMAND_MENU_SELECTED,
  function() ide:GetEditor():MarkerGotoPrev(BOOKMARK_MARKER) end)
