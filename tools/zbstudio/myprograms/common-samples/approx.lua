local common  = require("common")

local s1 = common.getApprox(7,14,50)
local s2 = common.getApprox(14,7,50)
local s3 = common.getApprox(14,6,50)
local s4 = common.getApprox(355,113,50)
local s5 = common.getApprox(307,113,50)

print(s1:len(), s1)
print(s2:len(), s2)
print(s3:len(), s3)
print(s4:len(), s4)
print(s5:len(), s5)
