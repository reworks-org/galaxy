require("turtle")
local crt = require("chartmap")
local cmp = require("complex")
local col = require("colormap")

local v1 = cmp.getNew(5,0)
local v2 = cmp.getNew(-2,0)

local M = {10, "25"}

local V0 = v1:getCenterMass(v1, M[1], v2, M[2])
local V1 = v1:getCenterMass(5, M[1], -2, M[2])
local V2 = v1:getCenterMass("5", M[1], "-2", M[2])
local V3 = v1:getCenterMass({v1, v2}, M)
local V4 = v1:getCenterMass({5, -2}, M)
local V5 = v1:getCenterMass({"5", "-2"}, M)

print("The center of mass is:", V0, V1, V2, V3, V4, V5)
