--[[ [previous](05-explain.lua) | [contents](00-contents.lua) | [next](07-numbers.lua)

# Lifespan of a light bulb

I was changing a light bulb the other day and my son noticed that it was rated for 2200 hours. He asked me how many years it would last. Let us write a program to figure it out.

First, we need to make some assumptions. As we sometimes do not have all the information we need to write a program, we can **estimate** missing information and capture it as an **assumption**. In this case, we assume that this is a bulb from a kitchen that is used for 6 hours a day on average.

To calculate the number of years, we can divide the number of hours the bulb is rated for (2200) by the number of hours in a day (6) and then divide by the number of days in a year (365). To write a script that does the calculation, we need to learn about numbers and operations on them. 
]]