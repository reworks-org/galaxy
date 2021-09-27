-- Copyright 2011-17 Paul Kulchenko, ZeroBrane LLC
-- authors: Lomtik Software (J. Winwood & John Labenski)
-- Luxinia Dev (Eike Decker & Christoph Kubisch)
---------------------------------------------------------

local ide = ide
local frame = ide.frame
local menuBar = frame.menuBar

------------------------
-- Interpreters and Menu

local targetDirMenu = ide:MakeMenu {
  {ID_PROJECTDIRCHOOSE, TR("Choose...")..KSC(ID_PROJECTDIRCHOOSE), TR("Choose a project directory")},
  {ID_PROJECTDIRFROMFILE, TR("Set From Current File")..KSC(ID_PROJECTDIRFROMFILE), TR("Set project directory from current file")},
}
local targetMenu = ide:MakeMenu {}
local debugMenu = ide:MakeMenu {
  { ID_RUN, TR("&Run")..KSC(ID_RUN), TR("Execute the current project/file") },
  { ID_RUNNOW, TR("Run As Scratchpad")..KSC(ID_RUNNOW), TR("Execute the current project/file and keep updating the code to see immediate results"), wx.wxITEM_CHECK },
  { ID_COMPILE, TR("&Compile")..KSC(ID_COMPILE), TR("Compile the current file") },
  { ID_STARTDEBUG, TR("Start &Debugging")..KSC(ID_STARTDEBUG), TR("Start or continue debugging") },
  { ID_ATTACHDEBUG, TR("&Start Debugger Server")..KSC(ID_ATTACHDEBUG), TR("Allow external process to start debugging"), wx.wxITEM_CHECK },
  { },
  { ID_STOPDEBUG, TR("S&top Process")..KSC(ID_STOPDEBUG), TR("Stop the currently running process") },
  { ID_DETACHDEBUG, TR("Detach &Process")..KSC(ID_DETACHDEBUG), TR("Stop debugging and continue running the process") },
  { ID_STEP, TR("Step &Into")..KSC(ID_STEP), TR("Step into") },
  { ID_STEPOVER, TR("Step &Over")..KSC(ID_STEPOVER), TR("Step over") },
  { ID_STEPOUT, TR("Step O&ut")..KSC(ID_STEPOUT), TR("Step out of the current function") },
  { ID_RUNTO, TR("Run To Cursor")..KSC(ID_RUNTO), TR("Run to cursor") },
  { ID_TRACE, TR("Tr&ace")..KSC(ID_TRACE), TR("Trace execution showing each executed line") },
  { ID_BREAK, TR("&Break")..KSC(ID_BREAK), TR("Break execution at the next executed line of code") },
  { },
  { ID_BREAKPOINT, TR("Breakpoint")..KSC(ID_BREAKPOINT), "", {
    { ID_BREAKPOINTTOGGLE, TR("Toggle Breakpoint")..KSC(ID_BREAKPOINTTOGGLE) },
    { ID_BREAKPOINTNEXT, TR("Go To Next Breakpoint")..KSC(ID_BREAKPOINTNEXT) },
    { ID_BREAKPOINTPREV, TR("Go To Previous Breakpoint")..KSC(ID_BREAKPOINTPREV) },
  } },
  { },
  { ID_CLEAROUTPUTENABLE, TR("C&lear Output Window")..KSC(ID_CLEAROUTPUTENABLE), TR("Clear the output window before compiling or debugging"), wx.wxITEM_CHECK },
  { ID_COMMANDLINEPARAMETERS, TR("Command Line Parameters...")..KSC(ID_COMMANDLINEPARAMETERS), TR("Provide command line parameters") },
  { ID_PROJECTDIR, TR("Project Directory"), TR("Set the project directory to be used"), targetDirMenu },
  { ID_INTERPRETER, TR("Lua &Interpreter"), TR("Set the interpreter to be used"), targetMenu },
}
menuBar:Append(debugMenu, TR("&Project"))
menuBar:Check(ID_CLEAROUTPUTENABLE, true)

-- older (<3.x) versions of wxwidgets may not have `GetLabelText`, so provide alternative
do
  local ok, glt = pcall(function() return debugMenu.GetLabelText end)
  if not ok or not glt then
    debugMenu.GetLabelText = function(self, ...) return wx.wxMenuItem.GetLabelText(self.GetLabel(self, ...)) end
  end
end
local debugMenuRunLabel = { [false]=debugMenu:GetLabelText(ID_STARTDEBUG), [true]=TR("Co&ntinue") }
local debugMenuStopLabel = { [false]=debugMenu:GetLabelText(ID_STOPDEBUG), [true]=TR("S&top Debugging") }

local interpreters
local function selectInterpreter(id)
  for id in pairs(interpreters) do
    menuBar:Check(id, false)
    menuBar:Enable(id, true)
  end
  menuBar:Check(id, true)
  menuBar:Enable(id, false)

  local changed = ide.interpreter ~= interpreters[id]
  if changed then
    if ide.interpreter then PackageEventHandle("onInterpreterClose", ide.interpreter) end
    if interpreters[id] then PackageEventHandle("onInterpreterLoad", interpreters[id]) end
  end

  ide.interpreter = interpreters[id]

  ide:GetDebugger():Shutdown()

  if ide.interpreter then
    ide.interpreter:UpdateStatus()
  else
    ide:SetStatus("", 4)
  end
  if changed then ReloadAPIs() end
end

function ProjectSetInterpreter(name)
  local id = IDget("debug.interpreter."..name)
  if id and interpreters[id] then
    selectInterpreter(id)
    return true
  else
    ide:Print(("Can't load interpreter '%s'; using the default interpreter instead."):format(name))
    local id = (
      -- interpreter is set and is (still) on the list of known interpreters
      IDget("debug.interpreter."..(ide.config.interpreter or ide.config.default.interpreter)) or
      -- otherwise use default interpreter
      ID("debug.interpreter."..ide.config.default.interpreter)
    )
    selectInterpreter(id)
  end
end

local function evSelectInterpreter(event)
  selectInterpreter(event:GetId())
end

function ProjectUpdateInterpreters()
  assert(ide.interpreters, "no interpreters defined")

  -- delete all existing items (if any)
  local items = targetMenu:GetMenuItemCount()
  for i = items, 1, -1 do
    targetMenu:Delete(targetMenu:FindItemByPosition(i-1))
  end

  local names = {}
  for file in pairs(ide.interpreters) do table.insert(names, file) end
  table.sort(names)

  interpreters = {}
  for _, file in ipairs(names) do
    local inter = ide.interpreters[file]
    local id = ID("debug.interpreter."..file)
    inter.fname = file
    interpreters[id] = inter
    targetMenu:Append(
      wx.wxMenuItem(targetMenu, id, inter.name, inter.description, wx.wxITEM_CHECK))
    frame:Connect(id, wx.wxEVT_COMMAND_MENU_SELECTED, evSelectInterpreter)
  end

  local id = (
    -- interpreter is set and is (still) on the list of known interpreters
    IDget("debug.interpreter."
      ..(ide.interpreter and ide.interpreters[ide.interpreter.fname] and ide.interpreter.fname
         or ide.config.interpreter or ide.config.default.interpreter)) or
    -- otherwise use default interpreter
    ID("debug.interpreter."..ide.config.default.interpreter)
  )
  selectInterpreter(id)
end

-----------------------------
-- Project directory handling

local function projChoose(event)
  local editor = ide:GetEditor()
  local fn = wx.wxFileName(
    editor and ide:GetDocument(editor):GetFilePath() or "")
  fn:Normalize() -- want absolute path for dialog

  local projectdir = ide:GetProject()
  local filePicker = wx.wxDirDialog(frame, TR("Choose a project directory"),
    projectdir ~= "" and projectdir or wx.wxGetCwd(), wx.wxDIRP_DIR_MUST_EXIST)
  if filePicker:ShowModal(true) == wx.wxID_OK then
    return ide:SetProject(filePicker:GetPath())
  end
  return false
end

frame:Connect(ID_PROJECTDIRCHOOSE, wx.wxEVT_COMMAND_MENU_SELECTED, projChoose)

local function projFromFile(event)
  local editor = ide:GetEditor()
  if not editor then return end
  local filepath = ide:GetDocument(editor):GetFilePath()
  if not filepath then return end
  local fn = wx.wxFileName(filepath)
  fn:Normalize() -- want absolute path for dialog

  if ide.interpreter then
    ide:SetProject(ide.interpreter:fprojdir(fn))
  end
end
frame:Connect(ID_PROJECTDIRFROMFILE, wx.wxEVT_COMMAND_MENU_SELECTED, projFromFile)
frame:Connect(ID_PROJECTDIRFROMFILE, wx.wxEVT_UPDATE_UI,
  function (event)
    local editor = ide:GetEditor()
    event:Enable(editor ~= nil and ide:GetDocument(editor):GetFilePath() ~= nil)
  end)

----------------------
-- Interpreter Running

local function getNameToRun(skipcheck)
  local editor = ide:GetEditor()
  if not editor then return end

  -- test compile it before we run it, if successful then ask to save
  -- only compile if lua api
  if editor.spec.apitype and
    editor.spec.apitype == "lua" and
    (not skipcheck) and
    (not ide.interpreter.skipcompile) and
    (not CompileProgram(editor, { reportstats = false })) then
    return
  end

  local doc = ide:GetDocument(editor)
  local name = ide:GetProjectStartFile() or doc:GetFilePath()
  if not SaveIfModified(editor) then return end
  if ide.config.editor.saveallonrun then SaveAll(true) end

  return wx.wxFileName(name or doc:GetFilePath())
end

local function runInterpreter(wfilename, withdebugger)
  ClearOutput()
  ide:GetOutput():Activate()

  ClearAllCurrentLineMarkers()
  if not wfilename or not ide.interpreter.frun then return end
  local pid = ide.interpreter:frun(wfilename, withdebugger)
  if pid then OutputEnableInput() end
  ide:SetLaunchedProcess(pid)
  return pid
end

function ProjectRun(skipcheck)
  local fname = getNameToRun(skipcheck)
  if not fname then return end
  return runInterpreter(fname)
end

local debuggers = {
  debug = "require('mobdebug').loop('%s',%d)",
  scratchpad = "require('mobdebug').scratchpad('%s',%d)"
}

function ProjectDebug(skipcheck, debtype)
  local debugger = ide:GetDebugger()
  if (debugger:IsConnected()) then
    if (debugger.scratchpad and debugger.scratchpad.paused) then
      debugger.scratchpad.paused = nil
      debugger.scratchpad.updated = true
      ide:GetConsole():SetRemote(nil) -- disable remote while Scratchpad running
    elseif (not debugger:IsRunning()) then
      debugger:Run()
    end
  else
    if not debugger:IsListening() then debugger:Listen() end
    local debcall = (debuggers[debtype or "debug"]):
      format(debugger:GetHostName(), debugger:GetPortNumber())
    local fname = getNameToRun(skipcheck)
    if not fname then return end
    return runInterpreter(fname, debcall) -- this may be pid or nil
  end
  return true
end

-----------------------
-- Actions

local BREAKPOINT_MARKER = StylesGetMarker("breakpoint")

frame:Connect(ID_BREAKPOINTTOGGLE, wx.wxEVT_COMMAND_MENU_SELECTED,
  function() ide:GetEditor():BreakpointToggle() end)
frame:Connect(ID_BREAKPOINTTOGGLE, wx.wxEVT_UPDATE_UI,
  function (event)
    local debugger = ide:GetDebugger()
    local editor = ide:GetEditorWithFocus(ide:GetEditor())
    event:Enable(ide.interpreter and ide.interpreter.hasdebugger and (not debugger.scratchpad)
      and (editor ~= nil) and (not editor:IsLineEmpty()))
  end)

frame:Connect(ID_BREAKPOINTNEXT, wx.wxEVT_COMMAND_MENU_SELECTED,
  function()
    local BPNSC = KSC(ID_BREAKPOINTNEXT):gsub("\t","")
    if not ide:GetEditor():MarkerGotoNext(BREAKPOINT_MARKER) and BPNSC == "F9" then
      local osx = ide.osname == "Macintosh"
      ide:Print(("You used '%s' shortcut that has been changed from toggling a breakpoint to navigating to the next breakpoint in the document.")
        :format(BPNSC))
      -- replace Ctrl with Cmd, but not in RawCtrl
      ide:Print(("To toggle a breakpoint, use '%s' or click in the editor margin.")
        :format(KSC(ID_BREAKPOINTTOGGLE):gsub("\t",""):gsub("%f[%w]Ctrl", osx and "Cmd" or "Ctrl")))
    end
  end)
frame:Connect(ID_BREAKPOINTPREV, wx.wxEVT_COMMAND_MENU_SELECTED,
  function() ide:GetEditor():MarkerGotoPrev(BREAKPOINT_MARKER) end)

frame:Connect(ID_BREAKPOINTNEXT, wx.wxEVT_UPDATE_UI,
  function (event) event:Enable(ide:GetEditor() ~= nil) end)
frame:Connect(ID_BREAKPOINTPREV, wx.wxEVT_UPDATE_UI,
  function (event) event:Enable(ide:GetEditor() ~= nil) end)

frame:Connect(ID_COMPILE, wx.wxEVT_COMMAND_MENU_SELECTED,
  function ()
    ide:GetOutput():Activate()
    CompileProgram(ide:GetEditor(), {
        keepoutput = ide:GetLaunchedProcess() ~= nil or ide:GetDebugger():IsConnected()
    })
  end)
frame:Connect(ID_COMPILE, wx.wxEVT_UPDATE_UI,
  function (event) event:Enable(ide:GetEditor() ~= nil) end)

frame:Connect(ID_RUN, wx.wxEVT_COMMAND_MENU_SELECTED, function () ProjectRun() end)
frame:Connect(ID_RUN, wx.wxEVT_UPDATE_UI,
  function (event)
    event:Enable(ide:GetDebugger():IsConnected() == nil and
                 ide:GetLaunchedProcess() == nil and
                 (ide.interpreter.frun ~= nil) and -- nil == no running from this interpreter
                 ide:GetEditor() ~= nil)
  end)

frame:Connect(ID_RUNNOW, wx.wxEVT_COMMAND_MENU_SELECTED,
  function (event)
    local debugger = ide:GetDebugger()
    if debugger.scratchpad then
      debugger:ScratchpadOff()
    else
      debugger:ScratchpadOn(ide:GetEditor())
    end
  end)
frame:Connect(ID_RUNNOW, wx.wxEVT_UPDATE_UI,
  function (event)
    local editor = ide:GetEditor()
    local debugger = ide:GetDebugger()
    -- allow scratchpad if there is no server or (there is a server and it is
    -- allowed to turn it into a scratchpad) and we are not debugging anything
    event:Enable((ide.interpreter) and (ide.interpreter.hasdebugger) and
                 (ide.interpreter.frun ~= nil) and -- nil == no running from this interpreter
                 (ide.interpreter.scratchextloop ~= nil) and -- nil == no scratchpad support
                 (editor ~= nil) and ((debugger:IsConnected() == nil or debugger.scratchable)
                 and ide:GetLaunchedProcess() == nil or debugger.scratchpad ~= nil))
    local isscratchpad = debugger.scratchpad ~= nil
    menuBar:Check(ID_RUNNOW, isscratchpad)
    local tool = ide:GetToolBar():FindTool(ID_RUNNOW)
    if tool and tool:IsSticky() ~= isscratchpad then
      tool:SetSticky(isscratchpad)
      ide:GetToolBar():Refresh()
    end
  end)

frame:Connect(ID_ATTACHDEBUG, wx.wxEVT_COMMAND_MENU_SELECTED,
  function (event)
    ide:GetDebugger():Listen(event:IsChecked()) -- start/stop listening
    if event:IsChecked() and ide.interpreter.fattachdebug then ide.interpreter:fattachdebug() end
  end)
frame:Connect(ID_ATTACHDEBUG, wx.wxEVT_UPDATE_UI,
  function (event)
    event:Enable(ide.interpreter and ide.interpreter.fattachdebug and true or false)
    ide.frame.menuBar:Check(event:GetId(), ide:GetDebugger():IsListening() and true or false)
  end)

frame:Connect(ID_STARTDEBUG, wx.wxEVT_COMMAND_MENU_SELECTED, function () ProjectDebug() end)
frame:Connect(ID_STARTDEBUG, wx.wxEVT_UPDATE_UI,
  function (event)
    local editor = ide:GetEditor()
    local debugger = ide:GetDebugger()
    event:Enable((ide.interpreter) and (ide.interpreter.hasdebugger) and
                 (ide.interpreter.frun ~= nil) and -- nil == no running from this interpreter
      ((debugger:IsConnected() == nil and ide:GetLaunchedProcess() == nil and editor ~= nil) or
       (debugger:IsConnected() ~= nil and not debugger:IsRunning())) and
      (not debugger.scratchpad or debugger.scratchpad.paused))
    local isconnected = debugger:IsConnected() ~= nil
    local label, other = debugMenuRunLabel[isconnected], debugMenuRunLabel[not isconnected]
    if debugMenu:GetLabelText(ID_STARTDEBUG) == wx.wxMenuItem.GetLabelText(other) then
      debugMenu:SetLabel(ID_STARTDEBUG, label..KSC(ID_STARTDEBUG))
    end
  end)

frame:Connect(ID_STOPDEBUG, wx.wxEVT_COMMAND_MENU_SELECTED,
  function () ide:GetDebugger():Stop() end)
frame:Connect(ID_STOPDEBUG, wx.wxEVT_UPDATE_UI,
  function (event)
    local debugger = ide:GetDebugger()
    event:Enable(debugger:IsConnected() ~= nil or ide:GetLaunchedProcess() ~= nil)
    local isdebugging = debugger:IsConnected() ~= nil
    local label, other = debugMenuStopLabel[isdebugging], debugMenuStopLabel[not isdebugging]
    if debugMenu:GetLabelText(ID_STOPDEBUG) == wx.wxMenuItem.GetLabelText(other) then
      debugMenu:SetLabel(ID_STOPDEBUG, label..KSC(ID_STOPDEBUG))
    end
  end)

frame:Connect(ID_DETACHDEBUG, wx.wxEVT_COMMAND_MENU_SELECTED,
  function () ide:GetDebugger():detach() end)
frame:Connect(ID_DETACHDEBUG, wx.wxEVT_UPDATE_UI,
  function (event)
    local debugger = ide:GetDebugger()
    event:Enable(debugger:IsConnected() ~= nil and (not debugger.scratchpad))
  end)

frame:Connect(ID_RUNTO, wx.wxEVT_COMMAND_MENU_SELECTED,
  function ()
    local editor = ide:GetEditor()
    ide:GetDebugger():RunTo(editor, editor:GetCurrentLine()+1)
  end)
frame:Connect(ID_RUNTO, wx.wxEVT_UPDATE_UI,
  function (event)
    local debugger = ide:GetDebugger()
    event:Enable((debugger:IsConnected() ~= nil) and (not debugger:IsRunning())
      and (ide:GetEditor() ~= nil) and (not debugger.scratchpad))
  end)

frame:Connect(ID_STEP, wx.wxEVT_COMMAND_MENU_SELECTED,
  function () ide:GetDebugger():Step() end)
frame:Connect(ID_STEP, wx.wxEVT_UPDATE_UI,
  function (event)
    local debugger = ide:GetDebugger()
    event:Enable((debugger:IsConnected() ~= nil) and (not debugger:IsRunning())
      and (ide:GetEditor() ~= nil) and (not debugger.scratchpad))
  end)

frame:Connect(ID_STEPOVER, wx.wxEVT_COMMAND_MENU_SELECTED,
  function () ide:GetDebugger():Over() end)
frame:Connect(ID_STEPOVER, wx.wxEVT_UPDATE_UI,
  function (event)
    local debugger = ide:GetDebugger()
    event:Enable((debugger:IsConnected() ~= nil) and (not debugger:IsRunning())
      and (ide:GetEditor() ~= nil) and (not debugger.scratchpad))
  end)

frame:Connect(ID_STEPOUT, wx.wxEVT_COMMAND_MENU_SELECTED,
  function () ide:GetDebugger():Out() end)
frame:Connect(ID_STEPOUT, wx.wxEVT_UPDATE_UI,
  function (event)
    local debugger = ide:GetDebugger()
    event:Enable((debugger:IsConnected() ~= nil) and (not debugger:IsRunning())
      and (ide:GetEditor() ~= nil) and (not debugger.scratchpad))
  end)

frame:Connect(ID_TRACE, wx.wxEVT_COMMAND_MENU_SELECTED,
  function () ide:GetDebugger():trace() end)
frame:Connect(ID_TRACE, wx.wxEVT_UPDATE_UI,
  function (event)
    local debugger = ide:GetDebugger()
    event:Enable((debugger:IsConnected() ~= nil) and (not debugger:IsRunning())
      and (ide:GetEditor() ~= nil) and (not debugger.scratchpad))
  end)

frame:Connect(ID_BREAK, wx.wxEVT_COMMAND_MENU_SELECTED,
  function ()
    local debugger = ide:GetDebugger()
    if debugger.server then
      debugger:Break()
      if debugger.scratchpad then
        debugger.scratchpad.paused = true
        ide:GetConsole():SetRemote(debugger:GetConsole())
      end
    end
  end)
frame:Connect(ID_BREAK, wx.wxEVT_UPDATE_UI,
  function (event)
    local debugger = ide:GetDebugger()
    event:Enable(debugger:IsConnected() ~= nil
      and (debugger:IsRunning()
           or (debugger.scratchpad and not debugger.scratchpad.paused)))
  end)

frame:Connect(ID_COMMANDLINEPARAMETERS, wx.wxEVT_COMMAND_MENU_SELECTED,
  function ()
    local params = ide:GetTextFromUser(TR("Enter command line parameters"),
      TR("Command line parameters"), ide.config.arg.any or "")
    -- params is `nil` when the dialog is canceled
    if params then ide:SetCommandLineParameters(params) end
  end)
frame:Connect(ID_COMMANDLINEPARAMETERS, wx.wxEVT_UPDATE_UI,
  function (event)
    local interpreter = ide:GetInterpreter()
    event:Enable(interpreter and interpreter.takeparameters and true or false)
  end)

-- save and restore command line parameters
ide:AddPackage("core.project", {
    AddCmdLine = function(self, params)
      local settings = self:GetSettings()
      local arglist = settings.arglist or {}
      PrependStringToArray(arglist, params, ide.config.commandlinehistorylength)
      settings.arglist = arglist
      self:SetSettings(settings)
    end,
    GetCmdLines = function(self) return self:GetSettings().arglist or {} end,

    onProjectLoad = function(self, project)
      local settings = self:GetSettings()
      if type(settings.arg) == "table" then
        ide:SetConfig("arg.any", settings.arg[project], project)
      end
      local interpreter = ide:GetInterpreter()
      if interpreter then interpreter:UpdateStatus() end
    end,
    onProjectClose = function(self, project)
      local settings = self:GetSettings()
      if type(settings.arg) ~= "table" then settings.arg = {} end
      if settings.arg[project] ~= ide.config.arg.any then
        settings.arg[project] = ide.config.arg.any
        self:SetSettings(settings)
      end
    end,
})
