function CMarkSymbols(code, pos, vars)
  local idtmpl = "[A-Za-z_][A-Za-z0-9_ ]*"
  local funcdeftmpl = "("..idtmpl.."[%*%s:~]+"..idtmpl:gsub(" ",":")..")%s*(%b())%s*%{"
  local isfndef = function(str, pos)
    local s,e,pref,cap,parms
    while true do
      s,e,pref,cap,parms = str:find("^(%s*)"..funcdeftmpl, pos)
      if (not s) then
        s,e,pref,cap,parms = str:find("([\r\n]%s*)"..funcdeftmpl, pos)
      end
      -- skip strange parameters and things of `else if ()` variety
      if parms and #parms > 0 and not (parms:find(idtmpl) or parms:find("%(%s*%)"))
      or cap and (cap:find("%sif%s*$") or cap:find("[^:]:[^:]")) then
        pos = s+#pref+#cap+#parms
      else
        break
      end
    end
    if s then return s+#pref,s+#pref+#cap-1,cap end
  end

  return coroutine.wrap(function()
      -- return a dummy token to produce faster result for quick typing
      coroutine.yield("String", "dummy", 1, {})
      while true do
        local fpos, lpos, name  = isfndef(code, pos)
        if not fpos then return end
        coroutine.yield("Function", name, fpos, {}, 1)
        pos = fpos + #name
      end
  end)
end

return nil -- not a real spec
