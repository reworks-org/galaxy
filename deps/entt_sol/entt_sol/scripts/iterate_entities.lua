local view = registry:runtime_view(Transform)
assert(view:size_hint() > 0)

local koopa = registry:create()
registry:emplace(koopa, Transform(100, -200))
transform = registry:get(koopa, Transform)
print('Koopa position = ' .. tostring(transform))

assert(view:size_hint() == 2)

view:each(function(entity)
  print('Remove Transform from entity: ' .. entity)
  registry:remove(entity, Transform)
end)

assert(view:size_hint() == 0)
