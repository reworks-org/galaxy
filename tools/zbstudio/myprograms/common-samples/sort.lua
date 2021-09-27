local common = require("common")

--[[
  This is how you can select a sorting algorithm
  [1] = QuickSort
  [2] = SelectSort
  [3] = BubbleSort
  There is no way that the program can /guess/ your data strure :D
]]
common.sortSet(2)

--[[
  Also add your own sorter like this and use it as the current.
  If the function or the ID are invalid it will fall back on using Qsort
]]
-- common.sortSet(common.sortAdd(function() return 0 end, "My sort"))

-- To list the sorting algorithms you can do the following
common.sortList()

--[[
  For sorting general items of objects supporting the />/ operator
  you do not need to provide column names that you sort on
]]
local tT = {8,2,9,3,7,6,1,10,5,3}

common.logTable(common.sortTable(tT), "General")

--[[
  Here we have a structure and the Lua table does not have the />/
  operator so a sequential key list must be provided.
  You can use the third argument as RAW sorted output to obtain the sorted keys.
  If the RAW flag is set to false. It will return a sorted copy of the original.
]]
local RAW = false
local tV = {
  ["a"] = {3, 7},
  ["b"] = {1, 2},
  ["c"] = {4, 6},
  ["e"] = {3, 9},
  ["f"] = {9, 5},
  ["d"] = {3, 3}
}

common.logTable(common.sortTable(tV, {1,2}, RAW), "Table")
