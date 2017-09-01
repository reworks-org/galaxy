entity =
{
	name = "Example",
	isDead = false,
	systems = 
	{
		[0] = "RenderSystem"
	},
	TransformComponent = 
	{
		x = 0,
		y = 0,
		angle = 0
	},
	SpriteComponent = 
	{
		group = 0,
		texture = "example.png"
	},
	ParallaxComponent =
	{
		texture = "parllax_combined.png",
		parallaxList = 
		{
			bottom = { layer = 0, x = 0, y = 0, w = 64, h = 205 },
			top = { layer = 2, x = 192, y = 0, w = 64, h = 205 }
		}
	}
}