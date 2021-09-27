--[[ [previous](15-r-strings.lua) | [contents](00-contents.lua) | [next](17-e-strings.lua)

# Exercise: numbers, operations, and priority

Replace underscores (__) in the tests below with values to make tests correct.
Hint: change `is(__, 2+3, '...')` to `is(5, 2+3, '...')` to pass the test.
After making the changes, run the tests to see the results.
]] 
require "testwell"
is(__, 1 + 2 * 3 - 1, 'Multiplication done before addition and subtraction')
is(__, (1 + 2) * 3 - 1, 'Parentheses change priority of operations')
is(__, (1 + 2) * (3 - 1), 'Parentheses are calculated left-to-right')
is(__, 12 / 4 / 3, 'Same priority operations [division] done left-to-right')
is(__, 12 / 4 * 3, 'Same priority operations [mixed] done left-to-right')
is(__, 12 / (4 * 3), 'Same priority operations [mixed] with grouping')
is(__, 14 - 6 - 3, 'Same priority operations [subtraction] done left-to-right')
is(__, 14 - (6 - 3), 'Same priority operations [subtraction] with grouping')
is(__, 2 * 1.5, 'Numbers may include a decimal point')
report()