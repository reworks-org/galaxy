-- Copyright (C) 2017 Deyan Dobromirov
-- An modules extensions functionalities library

if not debug.getinfo(3) then
  print("This is a module to extend other modules with.")
  print("Please try `local module = require('module').extend()`.")
  os.exit(1)
end

local complex = require("complex")
local matrix  = require("matrix")

local metaData   = {}
local extensions = {complex={},matrix={}}

metaData.__ipmtx = {{ 1, 0, 0, 0},
                    { 0, 0, 1, 0},
                    {-3, 3,-2,-1},
                    { 2,-2, 1, 1}}

function extensions.complex.getInterpolation(cSelf, nH, tI)
  local mtx = matrix.getNew(metaData.__ipmtx)
  local ftx = matrix.getNew({{tI.F [3], tI.F [1], tI.Fy [3], tI.Fy [1]},
                             {tI.F [4], tI.F [2], tI.Fy [4], tI.Fy [2]},
                             {tI.Fx[3], tI.Fx[1], tI.Fxy[3], tI.Fxy[1]},
                             {tI.Fx[4], tI.Fx[2], tI.Fxy[4], tI.Fxy[2]}});
  local nV, x, y = 0, cSelf:getParts()
  local mta = mtx:getMul(ftx):Mul(mtx:Trans())
  for iD = 1, 4 do for jD = 1, 4 do
    nV = (tonumber(mta(iD,jD) or 0)*x^(iD-1)*y^(jD-1))
  end; end; return nV
end

function extensions.matrix.complexNew(nRe, nIm)
  return complex.getNew(nRe, nIm)
end

function extensions.matrix.complexConvNew(vIn, ...)
  return complex.convNew(vIn, ...)
end

function extensions.matrix.complexGetRandom(nL, nU, vC)
  return complex.getRandom(nL, nU, vC)
end

return extensions
