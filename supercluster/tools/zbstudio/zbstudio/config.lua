local mac = ide.osname == 'Macintosh'
local win = ide.osname == "Windows"
if mac then
  local defaultsize = 11
  filetree.fontsize = defaultsize
  if ide.wxver >= "2.9.5" then
    editor.fontsize = defaultsize+1
    output.fontsize = defaultsize
    console.fontsize = defaultsize
  end

  editor.fontname = "Monaco"
  output.fontname = editor.fontname
  console.fontname = editor.fontname
else
  local defaultsize = 10
  editor.fontsize = defaultsize+1
  output.fontsize = defaultsize
  console.fontsize = defaultsize

  local sysid, major, minor = wx.wxGetOsVersion()
  editor.fontname =
    win and (major == 5 and "Courier New" or "Consolas") or "Monospace"
  output.fontname = editor.fontname
  console.fontname = editor.fontname
end

filetree.iconfontname = editor.fontname

singleinstance = not mac

unhidewindow = { -- allow unhiding of GUI windows
  -- 1 - show if hidden, 0 - ignore, 2 -- hide if shown
  ConsoleWindowClass = 2,
  -- ignore the following windows when "showing all"
  IME = 0,
  wxDisplayHiddenWindow = 0,
  -- window added when Microsoft.Windows.Common-Controls is enabled in the manifest
  tooltips_class32 = 0,
  ['MSCTFIME UI'] = 0,
  -- GLUT/opengl/SDL applications (for example, moai or love2d)
  GLUT = 1, FREEGLUT = 1, SDL_app = 1,
}
