--[[ [previous](16-e-numbers.lua) | [contents](00-contents.lua) | next

# Exercise: strings, concatenation, and formatting

Replace underscores (__) in the tests below with values to make tests correct.
Hint: change `is(__, 'a'..'b', '...')` to `is('ab', 'a'..'b', '...')` to pass the test.
After making the changes, run the tests to see the results.
]]
require "testwell"
is(__, '', 'Strings can be empty')
is(__, "Hello, World!", 'Strings in double quotes')
is(__, "Hello, " .. 'World!', 'Strings concatenated using ..')
is(__, "She said, 'I love to code'", 'Single quotes in double quotes')
is(__, 'a' .. 'b' .. 'c', 'Expressions with multiple concatenations')
is(__, 'a' .. ('b' .. 'c'), 'Parentheses used for grouping')
is(__, "I'm ".. 8 .." years old", 'Numbers convert to strings')
is(__, ("I'm %d years old"):format(8), 'Strings can be formatted')
is(__, ("%d is more than %d"):format(5, 2), 'Strings with placeholders')
report()