-- Copyright (C) 2011-2012 Paul Kulchenko
-- A turtle graphics library

if not debug.getinfo(3) then
  print("This is a module to load with `require('turtle')`.")
  os.exit(1)
end

require("wx")

local defaultSize = 450
local screenSizeX, screenSizeY = wx.wxDisplaySize()
screenSizeX = screenSizeX - 40 -- adjust position for default offset
screenSizeY = screenSizeY - 80
local inloop = wx.wxGetApp():IsMainLoopRunning()
local osname = wx.wxPlatformInfo.Get():GetOperatingSystemFamilyName()
local linux = osname == 'Unix'
-- don't use wxNullPen and wxNullBrush on Linux as either one generates
-- warnings with GTK (IA__gdk_gc_set_foreground: assertion `color != NULL' failed)

local frame
local bitmap
local mdc = wx.wxMemoryDC()

local sounds
local bitmaps
local key
local click
local autoUpdate
local showTurtles
local turtles = {}

local function pick(...)
  local topick = {}
  for _,value in ipairs({...}) do
    topick[value] = true
  end
  local all = #{...} == 0 -- pick all if nothing speficied
  for num,turtle in ipairs(turtles) do
    turtle.picked = all or topick[num] or false
  end
end

local function trtl(num)
  if num then return turtles[num] end

  local trtl = {
    picked = false,
    pendn = wx.wxPen(wx.wxBLACK_PEN),
    penup = wx.wxTRANSPARENT_PEN,
    font = wx.wxSWISS_FONT, -- thin TrueType font
    down = true,
    angle = 0.0,
    x = 0.0,
    y = 0.0,
  }
  table.insert(turtles, trtl)
  pick(#turtles)
  return trtl
end

local function each(callback, ...)
  local r1, r2, r3 -- expect to return no more than three values
  for _,turtle in ipairs(turtles) do
    if turtle.picked then
      if callback then r1, r2, r3 = callback(turtle, ...) end
    end
  end
  return r1, r2, r3
end

local function round(value) return math.floor(value+0.5) end

local function showTurtle(turtle)
  local pen = wx.wxPen(wx.wxBLACK_PEN)
  local size = turtle.pendn:GetWidth()
  pen:SetWidth(size > 5 and 5 or size)
  pen:SetColour(turtle.pendn:GetColour())

  mdc:SelectObject(bitmap)
  mdc:SetPen(pen)

  local angle = turtle.angle
  local dist, rad = 10, (math.pi / 180)
  local x1 = round(turtle.x + dist * math.cos(angle * rad))
  local y1 = round(turtle.y + dist * math.sin(angle * rad))
  local x2 = round(turtle.x + dist * math.cos((angle + 120) * rad))
  local y2 = round(turtle.y + dist * math.sin((angle + 120) * rad))
  local x3 = round(turtle.x + dist * math.cos((angle - 120) * rad))
  local y3 = round(turtle.y + dist * math.sin((angle - 120) * rad))

  mdc:DrawLine(x1, y1, x2, y2)
  mdc:DrawLine(x2, y2, x3, y3)
  mdc:DrawLine(x3, y3, x1, y1)

  if not linux then mdc:SetPen(wx.wxNullPen) end
  mdc:SelectObject(wx.wxNullBitmap)
end

local function snap()
  return bitmap:GetSubBitmap(wx.wxRect(0, 0, bitmap:GetWidth(), bitmap:GetHeight()))
end

local function undo(snapshot)
  if snapshot then bitmap = wx.wxBitmap(snapshot) end
end

local function updt(update)
  local curr = autoUpdate
  if update ~= nil then autoUpdate = update end

  local save
  if showTurtles then
    save = snap()
    each(showTurtle)
  end

  frame:Refresh()
  frame:Update()
  wx.wxGetApp():MainLoop()

  if showTurtles then undo(save) end

  return curr
end

local function reset()
  local size = frame:GetClientSize()
  local w,h = size:GetWidth(),size:GetHeight()
  bitmap = wx.wxBitmap(w,h)

  sounds = {}
  bitmaps = {}
  key = nil
  click = {}
  autoUpdate = true
  showTurtles = false

  turtles = {}
  trtl() -- add one turtle

  mdc:SelectObject(bitmap)
  mdc:SetDeviceOrigin(0, 0)
  mdc:Clear()
  mdc:SetDeviceOrigin(w/2, h/2)
  mdc:SelectObject(wx.wxNullBitmap)
end

-- paint event handler for the frame; called by wxEVT_PAINT
local function onPaint()
  -- must always create a wxPaintDC in a wxEVT_PAINT handler
  local dc = wx.wxPaintDC(frame)
  dc:DrawBitmap(bitmap, 0, 0, true)
  dc:delete() -- always delete any wxDCs created when done
end

local function open(name)
  name = name or "Turtle Graphics Window"
  -- if the window is open, then only reset it
  if frame then
    if name then frame:SetTitle(name) end
    return reset()
  end

  -- try to find a window by name
  local top = wx.wxGetApp():GetTopWindow()
  frame = top and top.FindWindowByLabel(name)
  -- OR create a new one now
    or wx.wxFrame(
    wx.NULL, -- no parent for toplevel windows
    wx.wxID_ANY, -- don't need a wxWindow ID
    name,
    wx.wxPoint(screenSizeX - defaultSize, screenSizeY - defaultSize),
    wx.wxSize(defaultSize, defaultSize),
    wx.wxDEFAULT_FRAME_STYLE + wx.wxSTAY_ON_TOP
    - wx.wxRESIZE_BORDER - wx.wxMAXIMIZE_BOX)
  frame:SetClientSize(defaultSize, defaultSize)

  frame:Connect(wx.wxEVT_CLOSE_WINDOW,
    function(event)
      if inloop then event:Skip() frame = nil else os.exit() end
    end)

  -- connect the paint event handler function with the paint event
  frame:Connect(wx.wxEVT_PAINT, onPaint)
  frame:Connect(wx.wxEVT_ERASE_BACKGROUND, function () end) -- do nothing

  frame:Connect(wx.wxEVT_KEY_DOWN, function (event) key = event:GetKeyCode() end)
  frame:Connect(wx.wxEVT_LEFT_DCLICK, function (event) click['l2'] = event:GetLogicalPosition(mdc) end)
  frame:Connect(wx.wxEVT_RIGHT_DCLICK, function (event) click['r2'] = event:GetLogicalPosition(mdc) end)
  frame:Connect(wx.wxEVT_LEFT_UP, function (event) click['lu'] = event:GetLogicalPosition(mdc) end)
  frame:Connect(wx.wxEVT_RIGHT_UP, function (event) click['ru'] = event:GetLogicalPosition(mdc) end)
  frame:Connect(wx.wxEVT_LEFT_DOWN, function (event) click['ld'] = event:GetLogicalPosition(mdc) end)
  frame:Connect(wx.wxEVT_RIGHT_DOWN, function (event) click['rd'] = event:GetLogicalPosition(mdc) end)

  if not inloop then
    local exitLoop = function () wx.wxGetApp():ExitMainLoop() end
    frame:Connect(wx.wxEVT_IDLE, exitLoop)
    frame:Connect(wx.wxEVT_TIMER, exitLoop)
  end

  -- call reset() before Show() as on MacOS it shows the window right away,
  -- which calls onPaint(), which expects reset() to be already called
  reset()

  frame:Show(true)
end

local function line(x1, y1, x2, y2)
  mdc:SelectObject(bitmap)

  each(function(turtle)
    mdc:SetPen(turtle.down and turtle.pendn or turtle.penup)
    mdc:DrawLine(x1, y1, x2, y2)
    if not linux then mdc:SetPen(wx.wxNullPen) end
  end)

  mdc:SelectObject(wx.wxNullBitmap)
  if autoUpdate then updt() end
end

local function rect(x, y, w, h, r)
  mdc:SelectObject(bitmap)

  each(function(turtle)
    mdc:SetPen(turtle.down and turtle.pendn or turtle.penup)
    if r then mdc:DrawRoundedRectangle(x, y, w, h, r)
    else mdc:DrawRectangle(x, y, w, h) end
    if not linux then mdc:SetPen(wx.wxNullPen) end
  end)

  mdc:SelectObject(wx.wxNullBitmap)
  if autoUpdate then updt() end
end

local function pixl(x, y)
  mdc:SelectObject(bitmap)

  each(function(turtle)
    mdc:SetPen(turtle.down and turtle.pendn or turtle.penup)
    mdc:DrawPoint(x, y)
    if not linux then mdc:SetPen(wx.wxNullPen) end
  end)

  mdc:SelectObject(wx.wxNullBitmap)
  if autoUpdate then updt() end
end

local function oval(x, y, w, h, color, start, finish)
  h = h or w
  start = start or 0
  finish = finish or 360
  if type(color) == "string" then color = wx.wxColour(color) end

  mdc:SelectObject(bitmap)

  each(function(turtle)
    mdc:SetPen(turtle.down and turtle.pendn or turtle.penup)
    mdc:SetBrush(
      color and wx.wxBrush(color, wx.wxSOLID) or wx.wxTRANSPARENT_BRUSH)
    mdc:DrawEllipticArc(x-w, y-h, w*2, h*2, start, finish)
    if not linux then mdc:SetBrush(wx.wxNullBrush) mdc:SetPen(wx.wxNullPen) end
  end)

  mdc:SelectObject(wx.wxNullBitmap)
  if autoUpdate then updt() end
end

local function move(dist, dy)
  if not dist then return end

  local dx -- if move is called with both parameters, move by vector
  if dy then dx = dist end

  mdc:SelectObject(bitmap)

  each(function(turtle)
    mdc:SetPen(turtle.down and turtle.pendn or turtle.penup)

    local dx = dx or dist * math.cos(turtle.angle * math.pi/180)
    local dy = dy or dist * math.sin(turtle.angle * math.pi/180)
    turtle.x, turtle.y = turtle.x+dx, turtle.y+dy
    mdc:DrawLine(round(turtle.x-dx), round(turtle.y-dy), round(turtle.x), round(turtle.y))
    if not linux then mdc:SetPen(wx.wxNullPen) end
  end)

  mdc:SelectObject(wx.wxNullBitmap)
  if autoUpdate then updt() end
end

local function fill(color, dx, dy)
  if not color then return end
  if type(color) == "string" then color = wx.wxColour(color) end

  mdc:SelectObject(bitmap)

  each(function(turtle)
    mdc:SetBrush(wx.wxBrush(color, wx.wxSOLID))
    mdc:FloodFill(turtle.x+(dx or 0), turtle.y+(dy or 0),
      turtle.pendn:GetColour(), wx.wxFLOOD_BORDER)
    if not linux then mdc:SetBrush(wx.wxNullBrush) end -- release the brush
  end)

  mdc:SelectObject(wx.wxNullBitmap)
  if autoUpdate then updt() end
end

local function text(text, angle, dx, dy)
  if not text then return end
  text = tostring(text)

  mdc:SelectObject(bitmap)

  local r1, r2 = each(function(turtle)
    mdc:SetFont(turtle.font)
    if angle then
      mdc:DrawRotatedText(text, turtle.x+(dx or 0), turtle.y+(dy or 0), angle)
    else
      mdc:DrawText(text, turtle.x+(dx or 0), turtle.y+(dy or 0))
    end
    return mdc:GetTextExtent(text)
  end)

  mdc:SelectObject(wx.wxNullBitmap)
  if autoUpdate then updt() end
  return r1, r2
end

local function load(file, x, y)
  if not file then return end
  if not wx.wxFileName(file):FileExists() then file = file .. ".png" end

  if not bitmaps[file] then
    bitmaps[file] = wx.wxBitmap()
    bitmaps[file]:LoadFile(file, wx.wxBITMAP_TYPE_ANY)
  end

  -- if the size is the same, then load the entire bitmap
  if bitmap:GetWidth() == bitmaps[file]:GetWidth() and
     bitmap:GetHeight() == bitmaps[file]:GetHeight() then
    bitmap:LoadFile(file, wx.wxBITMAP_TYPE_ANY)
  else
    each(function(turtle)
      mdc:SelectObject(bitmap)
      mdc:DrawBitmap(bitmaps[file], x or turtle.x, y or turtle.y, true)
      mdc:SelectObject(wx.wxNullBitmap)
    end)
  end
  if autoUpdate then updt() end
end

local function wipe()
  mdc:SelectObject(bitmap)
  mdc:Clear()
  mdc:SelectObject(wx.wxNullBitmap)
  if autoUpdate then updt() end
end

local function logf(value)
  local curr = mdc:GetLogicalFunction()
  if value and osname ~= 'Macintosh' then mdc:SetLogicalFunction(value) end
  return curr
end

local minwait = 50 -- ms
local gettime = pcall(require, "socket") and socket.gettime or os.time
local function wait(seconds)
  local stopat = gettime() + (seconds or 0)
  while true do
    local app = wx.wxGetApp()
    -- app:MainLoop() will abort as soon as it hits IDLE/TIMER event
    if not linux then app:MainLoop()
    elseif not seconds or 1000*seconds > minwait then
      wx.wxTimer(app):Start(minwait,true)
      app:MainLoop()
    end
    local stillneed = (stopat - gettime()) * 1000 -- milliseconds
    if not seconds then wx.wxMilliSleep(minwait)
    elseif stillneed <= 0 then return
    elseif stillneed > minwait then wx.wxMilliSleep(minwait)
    else wx.wxMilliSleep(math.floor(stillneed)); return
    end
  end
end

local function pndn() each(function(turtle) turtle.down = true end) end
local function pnup() each(function(turtle) turtle.down = false end) end
local function pncl(color)
  if type(color) == "string" then color = wx.wxColour(color) end

  local r = each(function(turtle, color)
    local curr = turtle.pendn:GetColour()
    if color then turtle.pendn:SetColour(color) end
    return curr
  end, color)
  if showTurtles then updt() end
  return r
end

local function colr(r, g, b, a)
  if not g or not b then return r end
  return wx.wxColour(math.floor(r), math.floor(g), math.floor(b), (a or wx.wxALPHA_OPAQUE))
end

local function rand(limit)
  return limit and (math.random(limit)-1) or math.random() end
local function ranc() return colr(rand(256),rand(256),rand(256)) end

local function turn(angle)
  if not angle then return end
  each(function(turtle) turtle.angle = (turtle.angle + angle) % 360 end)
  if showTurtles then updt() end
end

local function posn(...)
  return each(function(turtle, nx, ny)
    if not nx and not ny then return turtle.x, turtle.y end
    if nx then turtle.x = nx end
    if ny then turtle.y = ny end
  end, ...)
end

local function size(x, y)
  local size = frame:GetClientSize()
  if not x and not y then return size:GetWidth(), size:GetHeight() end
  local newx, newy = x or size:GetWidth(), y or size:GetHeight()
  frame:SetClientSize(newx, newy)
  frame:Move(screenSizeX - newx, screenSizeY - newy)
  reset()
end

local drawing = {
  show = function () showTurtles = true updt() end,
  hide = function () showTurtles = false updt() end,
  trtl = trtl,
  pick = pick,
  rant = function (num) -- get a turtle and initialize randomly
    local turtle = turtles[num]
    if turtle then pick(num) else turtle = trtl() end
    turn(rand(360))
    pncl(ranc())
    local x, y = size()
    posn(rand(x)-x/2, rand(y)-y/2)
    return turtle
  end,

  pndn = pndn,
  pnup = pnup,
  pnsz = function (...)
    local r = each(function(turtle, size)
      local curr = turtle.pendn:GetWidth()
      if size then turtle.pendn:SetWidth(size) end
      return curr
    end, ...)
    if showTurtles then updt() end
    return r
  end,
  pncl = pncl,
  pnpx = function (...)
    mdc:SelectObject(bitmap)
    each(function(turtle, x, y)
      local curr = turtle.pendn:GetColour()
      if osname == 'Macintosh' then
        local image = bitmap:GetSubBitmap(wx.wxRect(x,y,1,1)):ConvertToImage()
        curr = wx.wxColour(image:GetRed(0,0),image:GetGreen(0,0),image:GetBlue(0,0))
      else
        mdc:GetPixel(x, y, curr)
      end
      turtle.pendn:SetColour(curr)
    end, ...)
    mdc:SelectObject(wx.wxNullBitmap)
    if showTurtles then updt() end
  end,
  posn = posn,
  dist = function ()
    return each(function(turtle)
      local x, y = turtle.x, turtle.y
      return math.sqrt(x*x + y*y)
    end)
  end,
  turn = turn,
  bank = function () end,
  ptch = function () end,
  fill = fill,
  move = move,
  jump = function (dist) pnup() move(dist) pndn() end,
  back = function (dist) move(-dist) end,

  pixl = pixl,
  line = line,
  rect = rect,
  crcl = function (x, y, r, c, s, f) oval(x, y, r, r, c, s, f) end,
  oval = oval,

  font = function (...)
    return each(function(turtle, font)
      local curr = turtle.font:GetNativeFontInfoDesc()
      if font then
        if tonumber(font) == font then turtle.font:SetPointSize(font)
        elseif font == "bold"    then turtle.font:SetWeight(wx.wxFONTWEIGHT_BOLD)
        elseif font == "italic"  then turtle.font:SetStyle(wx.wxFONTSTYLE_ITALIC)
        elseif font == "normal"  then
          turtle.font:SetStyle(wx.wxFONTSTYLE_NORMAL)
          turtle.font:SetWeight(wx.wxFONTWEIGHT_NORMAL)
        else
          turtle.font:SetNativeFontInfo(font)
        end
      end
      return curr
    end, ...)
  end,
  colr = colr,
  char = function (char)
    if char then return type(char) == 'string' and char:byte() or char end
    local curr = key
    -- KEY_DOWN event is not sent to the frame on OSX, so provide a (partial) workaround;
    -- partial, because not all keys can be caught this way
    if not curr and osname == 'Macintosh' then
      for k = 32, 127 do
        if wx.wxGetKeyState(k) then
          -- as the same key generates upper- and lowercase codes,
          -- convert to uppercase as only one can be kept and it's consistent with KEY_DOWN
          -- behavior on Window and Linux
          curr = string.byte(string.char(k):upper())
          break
        end
      end
    end
    key = nil
    return curr
  end,
  clck = function (type)
    if not click[type] then return end
    local curr = click[type]
    click[type] = nil
    return curr.x, curr.y
  end,
  wipe = wipe,
  wait = wait,
  updt = updt,
  rand = rand,
  ranc = ranc,
  logf = logf,
  load = load,
  save = function (file) bitmap:SaveFile(file .. '.png', wx.wxBITMAP_TYPE_PNG) end,
  snap = snap,
  undo = undo,
  play = function (file)
    if not wx.wxFileName(file):FileExists() then file = file .. ".wav" end
    if not sounds[file] then sounds[file] = wx.wxSound(file) end
    sounds[file]:Play()
  end,
  text = text,
  time = function () return os.clock() end,
  open = open,
  done = function () frame:Close() end,
  size = size,
  zero = function (x, y)
    if x and y then mdc:SetDeviceOrigin(x, y) end
  end
}

math.randomseed(os.time())

for name, func in pairs(drawing) do
  -- install a proxy function that will run open() if needed
  -- and then install a proper function so that there is no
  -- performance penalty (other than the first call)
  -- this is needed to delay calling open() as much as possible
  -- to allow libraries derived from "turtle" to call open
  -- and provide their own window name
  _G[name] = function(...)
    if not frame and name ~= "open" then open() end
    for name, func in pairs(drawing) do _G[name] = func end
    return func(...)
  end
end
