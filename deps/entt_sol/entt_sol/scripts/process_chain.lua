local test_process = require("lua.test_process")

print(tostring(test_process))

my_proc = test_process.new("deploy_missile")
function my_proc:update(dt)
  self.fail()
end

scheduler:attach(
  test_process.new("open_silo"),
  my_proc,
  test_process.new("launch_missile")   -- this one is rejected because 'my_proc' fails (on purpose)
)

assert(not scheduler:empty())
assert(scheduler:size() == 1)
