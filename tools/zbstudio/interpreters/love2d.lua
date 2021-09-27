-- Copyright 2011-16 Paul Kulchenko, ZeroBrane LLC

local pathcache
local win = ide.osname == "Windows"
local mac = ide.osname == "Macintosh"

return {
  name = "LÖVE",
  description = "LÖVE game engine",
  api = {"baselib", "love2d"},
  frun = function(self,wfilename,rundebug)
    local projdir = self:fworkdir(wfilename)
    local love2d = ide.config.path.love2d or pathcache and pathcache[projdir]
    if love2d and not wx.wxFileExists(love2d) then
      ide:Print(("Can't find configured love2d executable: '%s'."):format(love2d))
      love2d = nil
    end
    if not love2d then
      local sep = win and ';' or ':'
      local default =
           win and (GenerateProgramFilesPath('love', sep)..sep)
        or mac and ('/Applications/love.app/Contents/MacOS'..sep)
        or ''
      local path = default
                 ..(os.getenv('PATH') or '')..sep
                 ..(GetPathWithSep(projdir))..sep
                 ..(os.getenv('HOME') and GetPathWithSep(os.getenv('HOME'))..'bin' or '')
      local paths = {}
      for p in path:gmatch("[^"..sep.."]+") do
        love2d = love2d or GetFullPathIfExists(p, win and 'love.exe' or 'love')
        table.insert(paths, p)
      end
      if not love2d then
        ide:Print("Can't find love2d executable in any of the following folders: "
          ..table.concat(paths, ", "))
        return
      end
      pathcache = pathcache or {}
      pathcache[projdir] = love2d
    end

    local main = 'main.lua'
    if not GetFullPathIfExists(projdir, main) then
      local altpath = wfilename:GetPath(wx.wxPATH_GET_VOLUME)
      local altname = GetFullPathIfExists(altpath, main)
      if altname and wx.wxMessageBox(
          ("Can't find '%s' file in the current project folder.\n"
           .."Would you like to switch the project directory to '%s'?"):format(main, altpath),
          "LÖVE interpreter",
          wx.wxYES_NO + wx.wxCENTRE, ide:GetMainFrame()) == wx.wxYES then
        ide:SetProject(altpath)
        ide:ActivateFile(altname) -- make sure that main.lua is also opened
        projdir = altpath
      else
        ide:Print(("Can't find '%s' file in the current project folder: '%s'.")
          :format(main, projdir))
        return
      end
    end

    if rundebug then
      ide:GetDebugger():SetOptions({runstart = ide.config.debugger.runonstart ~= false})
    end

    -- suppress hiding ConsoleWindowClass as this is used by Love console
    local uhw = ide.config.unhidewindow
    local cwc = uhw and uhw.ConsoleWindowClass
    if uhw then uhw.ConsoleWindowClass = 0 end

    local params = self:GetCommandLineArg()
    local cmd = ('"%s" "%s"%s%s'):format(love2d, projdir,
      params and " "..params or "", rundebug and ' -debug' or '')
    -- CommandLineRun(cmd,wdir,tooutput,nohide,stringcallback,uid,endcallback)
    return CommandLineRun(cmd,projdir,true,true,nil,nil,
      function() if uhw then uhw.ConsoleWindowClass = cwc end end)
  end,
  hasdebugger = true,
  scratchextloop = true,
  takeparameters = true,
}
