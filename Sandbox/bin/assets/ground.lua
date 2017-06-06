entity =
{
	name = "ground",

	SpriteComponent =
	{
		group = 2,
		isSpriteSheet = false,
		texture = "ground.png"
	},

	TransformComponent =
	{
		x = 550,
		y = 500,
		angle = 0
	},

	PhysicsComponent = 
	{
		x = 550,
		y = 500,
		bodyType = 0,
		fixedRotation = true,
		fixtureList =
		{
			fixtureA = 
			{
				w = 300,
				h = 50,
				angle = 0.0,
				density = 1,
				friction = 50,
				restitution = 0,
				id = "ground"
			}
		}
	}
}
