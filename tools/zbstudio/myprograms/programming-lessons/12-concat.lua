--[[ [previous](11-strings.lua) | [contents](00-contents.lua) | [next](13-format.lua)

# String concatenation

We will now use the interactive console to see what we can do with the strings. Go to the console and try to add two strings `'abc'` and `'def'` together. Let's use the `+` operation for that. [Click here](macro:shell('abc'+'def')) if you feel stuck.

Hm, that did not work. You should see an error in red that reads `"attempt to perform arithmetic on a string value"`. For adding strings together there is a separate operation, **concatenation**, with its own operator: `..` (two dots). Try to use this one instead of the plus sign to combine the strings. I will wait.

Much better. Now we can go back to the bulb lifespan calculation and modify it to output the message we like. Try running it in the shell or as a script.
]]

print('A bulb rated for 2200 hours will last ' .. (2200/6/365) .. ' years')