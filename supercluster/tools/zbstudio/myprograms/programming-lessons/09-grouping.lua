--[[ [previous](08-order.lua) | [contents](00-contents.lua) | [next](10-console.lua)

# Grouping of operations

We now know everything we need to calculate the lifespan of a light bulb. We can calculate the number of days by dividing 2200 by 6 and then calculate the number of years by dividing the result further by 365. Another way to do that would be to divide 2200 hours by the number of hours the bulb is used during one year, which we can calculate as `6*365`, but it would be a mistake to write this expression as `2200/6*365`. Take a minute to think why.

As the division and multiplication have the same priority, the expression is calculated left to right, with division being executed first, which leads to the wrong result. To fix it, we can use parentheses to group the operations.
]]

print(2200/6/365) -- lifespan of a light bulb rated for 2200 hours
print(2200/(6*365)) -- same result with a different way to calculate it