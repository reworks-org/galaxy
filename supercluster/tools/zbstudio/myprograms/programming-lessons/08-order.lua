--[[ [previous](07-numbers.lua) | [contents](00-contents.lua) | [next](09-grouping.lua)

# Order of operations

So far the **expressions** we have worked with have been simple ones, with only one operation. Can we use more than one? Yes, but we have to follow some rules that regulate the order of operations to avoid ambiguity. 

The general rules of the order of operations are simple:
(1) first operations in parentheses inside out,
(2) then multiplication and division left to right,
(3) then addition and subtraction also left to right.

This means that the expression `1+(4-2)*3` will be calculated with these steps:
- operation in parentheses will be done first (rule 1): `1+(2)*3`
- then multiplication (rule 2): `1+6`
- and finally addition (rule 3): `7`
]]