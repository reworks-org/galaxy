--[[ [previous](04-save-open.lua) | [contents](00-contents.lua) | [next](06-output.lua)

# Running programs

It would not be that interesting if you could only edit your programs. You want your programs to **do** something, don't you? You can do that by **running** (or **executing**) your program. 

The page you are reading right now is a script that you can execute. Press `F6` or go to `Project | Run` menu at the top of the window to run it. It will calculate and print a square root of 612.
]]

function sqrt(x)
  local function good(guess) return math.abs((guess^2) - x) < 1e-5 and guess end
  local function better(guess) return (guess + x / guess) / 2 end
  local function try(guess) return good(guess) or try(better(guess)) end
  return try(10)
end
print(sqrt(612))