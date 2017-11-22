//
//  Defines.hpp
//  rework
//
//  Created by reworks on 21/11/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_DEFINES_HPP_
#define REWORK_DEFINES_HPP_

///
/// Shortcuts to use.
///

#define vfs Locator::get<VFS>
#define window Locator::get<Window>
#define world Locator::get<World>
#define stateManager Locator::get<StateManager>
#define fontManager Locator::get<FontManager>
#define audioManager Locator::get<AudioManager>
#define b2dManager Locator::get<Box2DManager>
#define debugManager Locator::get<DebugManager>
#define textureAtlas Locator::get<TextureAtlas>
#define camera Locator::get<Camera>

#endif