bowser = registry:create()
assert(bowser == 0 and registry:size() == 1)
registry:emplace(bowser, Transform(5, 6))
assert(registry:has(bowser, Transform))
assert(registry:has(bowser, Transform.type_id()))

assert(not registry:any_of(bowser, -1, -2))

transform = registry:get(bowser, Transform)
transform.x = transform.x + 10
print('Bowser position = ' .. tostring(transform))
