entity =
{
	special = true,
	name = "player",
	TransformComponent =
	{
		x = 250,
		y = 100,
		width = 64,
		height = 205,
		angle = 0.0
	},
	SpriteComponent =
	{
		spriteName = "playerWalkFrame1",
		layer = 1
	},
	RenderableComponent = 
	{
	},
	AnimationComponent =
	{
		paused = false,
		defaultAnim = "walking",
		Animations =
		{
			walking =
			{
				looped = true,
				speed = 1.0,
				timePerFrame = 100,
				totalFrames = 4,
				frames = 
				{
					[0] = "playerWalkFrame1",
					[1] = "playerWalkFrame2",
					[2] = "playerWalkFrame3",
					[3] = "playerWalkFrame4"
				}
			}	
		}
	},
	PhysicsComponent = 
	{
		x = 250,
		y = 100,
		bodyType = 2,
		fixedRotation = true,
		fixtureList =
		{
			fixtureA = 
			{
				w = 64,
				h = 205,
				angle = 0,
				density = 1,
				friction = 1,
				restitution = 0,
				id = "person"
			}
		}
	}
}