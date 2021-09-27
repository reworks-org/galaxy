--[[ [previous](12-concat.lua) | [contents](00-contents.lua) | [next](14-r-numbers.lua)

# String formatting

If you look at the output of your script, you may not like the long number that was returned as part of the answer. When you write a program, you need to think not only how the **computer** is going to execute it, but also how **users** are going to use the program: what they need to enter and what they will see.

In this case we will **format** the string. Every string may include placeholders and provides a special way (`format` method) to put values in those placeholders. Placeholders start with `%` (percent sign). Here is a way to include a number: `print(("I'm %d years old"):format(8))`. Try it in the console. Type it; don't copy!

We can now format our message using the number placeholders: `%d` for integers (like `2200)` and `%f` for decimals (like `1.5`).
]]

print(('A bulb rated for %d hours will last about %.1f years'):format(2200, 2200/6/365))