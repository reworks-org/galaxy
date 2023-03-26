local example = {}

-- self.owner (entt::registry)
-- self.dispatcher (entt::dispatcher)
-- self.id (read only)

function example:construct(self)
end

-- Called in system update() loop.
function example:update(self)
end

function example:destruct(self)
end

return example