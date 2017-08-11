entity =
{
	name = "ground",
	isDead = false,
	systems = 
	{
		[0] = "PhysicsSystem",
		[1] = "RenderSystem"
	},

	SpriteComponent =
	{
		group = 2,
		texture = "ground.png"
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
