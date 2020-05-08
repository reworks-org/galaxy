entity =
{
	special = true,
	name = "ground",
	TransformComponent =
	{
		x = 0,
		y = 700,
		width = 720,
		height = 50,
		angle = 0.0
	},
	SpriteComponent =
	{
		spriteName = "ground",
		layer = 1
	},
	RenderableComponent = 
	{
	},
	PhysicsComponent = 
	{
		x = 0,
		y = 700,
		bodyType = 0,
		fixedRotation = true,
		fixtureList =
		{
			fixtureA = 
			{
				w = 720,
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