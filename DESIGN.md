Resources are designed to be loaded up front. This is fine since we aren't using the level of assets of i.e. a 3D game.
- This is similar in style to how games like minecraft and factorio loads its data.

A game will have a scene. This is usually for dividing up the game into something like photoshop layers.
I.e. you could have one for the menu, the game, the game hud.
	- Shared assets.
	- Communication is done through events or signals.
	- Separate entities, systems, ui, event handler, etc.
	- https://coffeebraingames.wordpress.com/2017/10/15/multi-scene-development-in-unity/


https://github.com/cocos2d/cocos2d-x/blob/v4/cocos/2d/CCScene.h
https://github.com/cocos2d/cocos2d-x/blob/v4/cocos/2d/CCNode.h
https://github.com/cocos2d/cocos2d-x/tree/v4/cocos/renderer
https://github.com/W4RH4WK/Anker/blob/main/code/anker/core/anker_scene_node.hpp


Editor needs to be an "overlay": i.e. editor stores a copy of the system i.e. scenemanager, and only during "run" do we copy it over.
