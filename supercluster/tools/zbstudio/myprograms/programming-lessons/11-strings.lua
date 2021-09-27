--[[ [previous](10-console.lua) | [contents](00-contents.lua) | [next](12-concat.lua)

# Strings

While we got the result we wanted (the bulb will last for approximately one year), the output was a bit boring; just a number without any explanation. As a user -- and any programmer should be able to see the program being developed from the perspective of a user -- I would prefer to see something more descriptive, like `A bulb rated for 2200 hours will last 1.0045662100457 years`. To do this we need to use **strings**.

A string is a text surrounded by double or single quotes. For example, `"Hello, World!"`, `'year'`, `"Don't do it!"`, and the like. `"Wrong'` and `'incorrect"` are two examples of invalid strings, because the surrounding quotes do not match.
]]

print("my long string in double quotes") -- experiment with
print('another string in single quotes') -- different strings here