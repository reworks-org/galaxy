entity =
{
	name = "person",

	TransformComponent =
	{
		x = 650,
		y = 200,
		angle = 0
	},

	PhysicsComponent = 
	{
		x = 650,
		y = 200,
		bodyType = 2,
		w = 64,
		h = 205,
		density = 1,
		friction = 1,
		restitution = 0,
		fixedRotation = true
	},

	EventComponent = 
	{
	},

	SpriteComponent = 
	{
		group = 3,
		isSpriteSheet = false,
		texture = "foo.png"
	},

	AnimationComponent =
	{
		speed = 100,
		isPaused = false,
		isLooped = true,
		defaultAnim = "walking",
		Animations =
		{
			walking =
			{
				frame1 = { x = 0, y = 0, w = 64, h = 205 },
				frame2 = { x = 64, y = 0, w = 64, h = 205 },
				frame3 = { x = 128, y = 0, w = 64, h = 205 },
				frame4 = { x = 192, y = 0, w = 64, h = 205 }
			}
		}
	}
}