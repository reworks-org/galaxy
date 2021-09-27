function move_fwd(entity)
	local transform = galaxy_current_world:get_transform(entity)
	transform:move(0.0, -10.0)
end

function move_back(entity)
	local transform = galaxy_current_world:get_transform(entity)
	transform:move(0.0, 10.0)
end

function move_left(entity)
	local transform = galaxy_current_world:get_transform(entity)
	transform:move(-10.0, 0.0)
end

function move_right(entity)
	local transform = galaxy_current_world:get_transform(entity)
	transform:move(10.0, 0.0)
end

function rotate_left(entity)
	local transform = galaxy_current_world:get_transform(entity)
	transform:rotate(-2.0)
end

function rotate_right(entity)
	local transform = galaxy_current_world:get_transform(entity)
	transform:rotate(2.0)
end

function reset(entity)
	local transform = galaxy_current_world:get_transform(entity)
	transform:rotate(0.0)
	transform:set_pos(0.0, 0.0)
end