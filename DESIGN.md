Resources are designed to be loaded up front. This is fine since we aren't using the level of assets of i.e. a 3D game.
- This is similar in style to how games like minecraft and factorio loads its data.

Scenes:
- Each scene has its own events, entities, ui, etc.
- But shared systems, like rendersystem / playercontroller, assets and resources.
- You might have a scene for a fullscreen map, menu, game, battle, etc.
- scenemanager would probably have some kind of playercontroller -> stores things like current terrain, current party, etc.
- on scene load would create entities or other stuff for current battle in a battle scene for example.
- individual scenes of different types? or a script call to lua that does stuff for you on scene load/unload.
- attach a script to a scene on construction -> register with lua as an event that can be called on (un)load.
