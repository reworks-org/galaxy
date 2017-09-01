entity =
{
	name = "person",
	isDead = false,
	systems = 
	{
		[0] = "PhysicsSystem",
		[1] = "RenderSystem",
		[2] = "MoveSystem",
		[3] = "EventSystem",
		[4] = "AnimationSystem"
	},
	CollisionComponent =
	{
		w = 64,
		h = 205
	},
	PhysicsComponent = 
	{
		x = 650,
		y = 200,
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
	},

	EventComponent = 
	{
	},

	SpriteComponent = 
	{
		group = 3,
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
			},
			walktwo =
			{
				frame1 = { x = 0, y = 0, w = 64, h = 205 },
				frame2 = { x = 64, y = 0, w = 64, h = 205 },
				frame3 = { x = 128, y = 0, w = 64, h = 205 },
				frame4 = { x = 192, y = 0, w = 64, h = 205 }
			}
		}
	}
}