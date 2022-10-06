ui =
{
	themes =
	{
		menuTheme =
		{
			colour =
			{
				r = 255,
				g = 255,
				b = 255,
				a = 255
			},
			font = "GameOver32",
			masterTexture = "ui/RPG_GUI_v1.png",
			textureRects =
			{
				panel =
				{
					x = 1025,
					y = 0,
					w = 512,
					h = 512
				},
				button =
				{
					x = 13,
					y = 126,
					w = 285,
					h = 56
				},
				buttonPressed =
				{
					x = 13,
					y = 204,
					w = 285,
					h = 56
				},
				buttonHovered =
				{
					x = 13,
					y = 282,
					w = 285,
					h = 56
				}
			}
		},
		tooltipTheme =
		{
			colour =
			{
				r = 0,
				g = 0,
				b = 0,
				a = 255
			},
			font = "GameOver66",
			masterTexture = "ui/RPG_GUI_v1.png",
			textureRects =
			{
				tooltip =
				{
					x = 118,
					y = 451,
					w = 80,
					h = 80
				}
			}
		}
	},
	panels =
	{
		menuPanel =
		{
			bounds = 
			{
				x = 640,
				y = 0,
				w = 640,
				h = 720
			},
			theme = "menuTheme",
			themeTextureID = "panel",
			widgets =
			{
				new_Button = 
				{
					theme = "menuTheme",
					x = 0,
					y = 10,
					defaultTexture = "button",
					pressedTexture = "buttonPressed",
					hoverTexture = "buttonHovered",
					tooltip = 
					{
						text = "Start a new game.",
						texture = "tooltip",
						sectionL = 6,
						sectionR = 74,
						theme = "tooltipTheme"
					}
				},
				load_Button = 
				{
					theme = "menuTheme",
					x = 0,
					y = 70,
					defaultTexture = "button",
					pressedTexture = "buttonPressed",
					hoverTexture = "buttonHovered",
					tooltip = 
					{
						text = "Continue your game.",
						texture = "tooltip",
						sectionL = 6,
						sectionR = 74,
						theme = "tooltipTheme"
					}
				},
				options_Button = 
				{
					theme = "menuTheme",
					x = 0,
					y = 130,
					defaultTexture = "button",
					pressedTexture = "buttonPressed",
					hoverTexture = "buttonHovered",
					tooltip = 
					{
						text = "Configure game settings.",
						texture = "tooltip",
						sectionL = 6,
						sectionR = 74,
						theme = "tooltipTheme"
					}
				},
				quit_Button = 
				{
					theme = "menuTheme",
					x = 0,
					y = 190,
					defaultTexture = "button",
					pressedTexture = "buttonPressed",
					hoverTexture = "buttonHovered",
					tooltip = 
					{
						text = "Quit the game.",
						texture = "tooltip",
						sectionL = 6,
						sectionR = 74,
						theme = "tooltipTheme"
					}
				}
			}
		}
	}
}