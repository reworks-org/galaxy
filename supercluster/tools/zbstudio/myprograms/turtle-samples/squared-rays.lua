-- Copyright (C) 2011-2012 Paul Kulchenko
-- A complete standalone example of using wx library to draw something

require("wx")

local frame = wx.wxFrame(
  wx.NULL, -- no parent for toplevel windows
  wx.wxID_ANY, -- don't need a wxWindow ID
  "Turtle Graph Window",
  wx.wxDefaultPosition,
  wx.wxSize(450, 450),
  wx.wxDEFAULT_FRAME_STYLE + wx.wxSTAY_ON_TOP 
  - wx.wxRESIZE_BORDER - wx.wxMAXIMIZE_BOX)

frame:Connect(wx.wxEVT_CLOSE_WINDOW, function() os.exit() end)
frame:Show(true)

local bitmap
local mdc = wx.wxMemoryDC()

local function reset ()
  local size = frame:GetClientSize()
  local w,h = size:GetWidth(),size:GetHeight()
  bitmap = wx.wxBitmap(w,h)

  mdc:SetDeviceOrigin(w/2, h/2)
  mdc:SelectObject(bitmap)
  mdc:Clear()
  mdc:SetPen(wx.wxBLACK_PEN)
  mdc:SetFont(wx.wxSWISS_FONT) -- thin TrueType font
  mdc:SelectObject(wx.wxNullBitmap)
end

reset()

-- paint event handler for the frame that's called by wxEVT_PAINT
function OnPaint(event)
  -- must always create a wxPaintDC in a wxEVT_PAINT handler
  local dc = wx.wxPaintDC(frame)
  dc:DrawBitmap(bitmap, 0, 0, true)
  dc:delete() -- ALWAYS delete() any wxDCs created when done
end

local exit = true
-- connect the paint event handler function with the paint event
frame:Connect(wx.wxEVT_PAINT, OnPaint)
frame:Connect(wx.wxEVT_ERASE_BACKGROUND, function () end) -- do nothing
frame:Connect(wx.wxEVT_IDLE, 
  function () if exit then wx.wxGetApp():ExitMainLoop() end end)

local autoUpdate = true
local function updt (update)
  local curr = autoUpdate
  if update ~= nil then autoUpdate = update end

  frame:Refresh()
  frame:Update()
  wx.wxGetApp():MainLoop()

  return curr
end

local function wait (seconds)
  if seconds then
    wx.wxMilliSleep(seconds*1000)
  else
    exit = false
    wx.wxGetApp():MainLoop()
  end
end

local function save (file)
  bitmap:SaveFile(file .. '.png', wx.wxBITMAP_TYPE_PNG)
end

local function line (x1, y1, x2, y2)
  mdc:SelectObject(bitmap)
  mdc:DrawLine(x1, y1, x2, y2)
  mdc:SelectObject(wx.wxNullBitmap)
  if autoUpdate then updt() end
end

local shift = 5
local side = 200
for i=0, side/shift do
  line(-side, i*shift, side, -i*shift)
  line(-side, -i*shift, side,  i*shift)
  line(i*shift, -side, -i*shift, side)
  line(-i*shift, -side,  i*shift, side)
  wait(0.05)
end

save("squared-rays")

wait()
