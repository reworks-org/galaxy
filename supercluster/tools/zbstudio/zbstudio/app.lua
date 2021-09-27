return {
  postinit = function ()
    local ide = ide
    local bundle = wx.wxIconBundle()
    local files = FileSysGetRecursive(ide:GetAppName().."/res", false, "*.ico")
    local icons = 0
    for i,file in ipairs(files) do
      icons = icons + 1
      bundle:AddIcon(file, wx.wxBITMAP_TYPE_ICO)
    end
    if icons > 0 then ide.frame:SetIcons(bundle) end

    -- load myprograms/welcome.lua if exists and no projectdir
    local projectdir = ide.config.path.projectdir
    if (not projectdir or string.len(projectdir) == 0
        or not wx.wxFileName(projectdir):DirExists()) then
      local home = wx.wxGetHomeDir():gsub("[\\/]$","")
      for _,dir in pairs({home, home.."/Desktop", ""}) do
        local fn = wx.wxFileName("myprograms/welcome.lua")
        -- normalize to absolute path
        if fn:Normalize(wx.wxPATH_NORM_ALL, dir) and fn:FileExists() then
          LoadFile(fn:GetFullPath(),nil,true)
          ide:SetProject(fn:GetPath(wx.wxPATH_GET_VOLUME))
          break
        end
      end
    end
  end,
  
  stringtable = {
    editor = "ZeroBrane Studio",
    about = "About ZeroBrane Studio",
    editormessage = "ZeroBrane Studio Message",
    statuswelcome = "Welcome to ZeroBrane Studio",
    settingsapp = "ZeroBraneStudio",
    settingsvendor = "ZeroBraneLLC",
    logo = "res/zerobrane.png",
    help = "zerobranestudio",
  },
}
