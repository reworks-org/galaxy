entity=
{
	name="menu",
	isDead = false,
	systems = 
	{
		[0] = "AudioSystem",
		[1] = "RenderSystem"
	},
	TransformComponent=
	{
		x=0,
		y=0,
		angle=0
	},
	SpriteComponent=
	{
		group=0,
		texture="bg2.png"
	},
	TextComponent=
	{
		x=0,
		y=0,
		group=1,
		font="GameOver",
		text="Hello, World!",
		size=72,
		colour=
		{
			r=255,
			g=0,
			b=0,
			a=255
		},
		style=0
	},
	MusicComponent =
	{
		bg =
		{
			file = "PleasingGuns.ogg",
			looping = true
		}
	}
}
