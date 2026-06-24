///
/// LuaRaylib.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include <entt/locator/locator.hpp>

#include <Raylib.hpp>

#include "../Lua.hpp"

namespace galaxy
{
	void Lua::inject_raylib() noexcept
	{
		auto& lua = entt::locator<sol::state>::value();

		lua.new_usertype<ray::Vector2>(
			"rayVector2",
			sol::call_constructor,
			sol::factories(
				[]() {
					return ray::Vector2 {};
				},
				[](float x, float y) {
					return ray::Vector2 {x, y};
				}
			),
			"x",
			&ray::Vector2::x,
			"y",
			&ray::Vector2::y
		);
		lua.new_usertype<ray::Vector3>(
			"rayVector3",
			sol::call_constructor,
			sol::factories(
				[]() {
					return ray::Vector3 {};
				},
				[](float x) {
					return ray::Vector3 {x, 0, 0};
				},
				[](float x, float y) {
					return ray::Vector3 {x, y, 0};
				},
				[](float x, float y, float z) {
					return ray::Vector3 {x, y, z};
				}
			),
			"x",
			&ray::Vector3::x,
			"y",
			&ray::Vector3::y,
			"z",
			&ray::Vector3::z
		);
		lua.new_usertype<ray::Vector4>(
			"rayVector4",
			sol::call_constructor,
			sol::factories(
				[]() {
					return ray::Vector4 {};
				},
				[](float x) {
					return ray::Vector4 {x, 0, 0, 0};
				},
				[](float x, float y) {
					return ray::Vector4 {x, y, 0, 0};
				},
				[](float x, float y, float z) {
					return ray::Vector4 {x, y, z, 0};
				},
				[](float x, float y, float z, float w) {
					return ray::Vector4 {x, y, z, w};
				}
			),
			"x",
			&ray::Vector4::x,
			"y",
			&ray::Vector4::y,
			"z",
			&ray::Vector4::z,
			"w",
			&ray::Vector4::w
		);
		lua.new_usertype<ray::Matrix>(
			"rayMatrix",
			sol::call_constructor,
			sol::factories(
				[]() {
					return ray::Matrix {};
				},
				[](float _m0,
				   float _m1,
				   float _m2,
				   float _m3,
				   float _m4,
				   float _m5,
				   float _m6,
				   float _m7,
				   float _m8,
				   float _m9,
				   float _m10,
				   float _m11,
				   float _m12,
				   float _m13,
				   float _m14,
				   float _m15) {
					return ray::Matrix {_m0, _m1, _m2, _m3, _m4, _m5, _m6, _m7, _m8, _m9, _m10, _m11, _m12, _m13, _m14, _m15};
				}
			),
			"m0",
			&ray::Matrix::m0,
			"m1",
			&ray::Matrix::m1,
			"m2",
			&ray::Matrix::m2,
			"m3",
			&ray::Matrix::m3,
			"m4",
			&ray::Matrix::m4,
			"m5",
			&ray::Matrix::m5,
			"m6",
			&ray::Matrix::m6,
			"m7",
			&ray::Matrix::m7,
			"m8",
			&ray::Matrix::m8,
			"m9",
			&ray::Matrix::m9,
			"m10",
			&ray::Matrix::m10,
			"m11",
			&ray::Matrix::m11,
			"m12",
			&ray::Matrix::m12,
			"m13",
			&ray::Matrix::m13,
			"m14",
			&ray::Matrix::m14,
			"m15",
			&ray::Matrix::m15
		);
		lua.new_usertype<ray::Color>(
			"rayColor",
			sol::call_constructor,
			sol::factories(
				[]() {
					return ray::Color {};
				},
				[](unsigned char r, unsigned char g, unsigned char b) {
					return ray::Color {r, g, b, 255};
				},
				[](unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
					return ray::Color {r, g, b, a};
				}
			),
			"r",
			&ray::Color::r,
			"g",
			&ray::Color::g,
			"b",
			&ray::Color::b,
			"a",
			&ray::Color::a
		);
		lua.new_usertype<ray::Rectangle>(
			"rayRectangle",
			sol::call_constructor,
			sol::factories(
				[]() {
					return ray::Rectangle {};
				},
				[](const ray::Rectangle& r) {
					return ray::Rectangle {r.x, r.y, r.width, r.height};
				},
				[](float x, float y, float width, float height) {
					return ray::Rectangle {x, y, width, height};
				}
			),
			"x",
			&ray::Rectangle::x,
			"y",
			&ray::Rectangle::y,
			"width",
			&ray::Rectangle::width,
			"height",
			&ray::Rectangle::height
		);
		lua.new_usertype<ray::Image>(
			"Image",
			sol::no_constructor,
			"data",
			&ray::Image::data,
			"width",
			&ray::Image::width,
			"height",
			&ray::Image::height,
			"mipmaps",
			&ray::Image::mipmaps,
			"format",
			&ray::Image::format
		);
		lua.new_usertype<ray::Texture2D>(
			"rayTexture2D",
			sol::no_constructor,
			"id",
			&ray::Texture2D::id,
			"width",
			&ray::Texture2D::width,
			"height",
			&ray::Texture2D::height,
			"mipmaps",
			&ray::Texture2D::mipmaps,
			"format",
			&ray::Texture2D::format
		);
		lua.new_usertype<ray::RenderTexture2D>(
			"rayRenderTexture2D",
			sol::no_constructor,
			"id",
			&ray::RenderTexture2D::id,
			"texture",
			&ray::RenderTexture2D::texture,
			"depth",
			&ray::RenderTexture2D::depth
		);

		lua.new_usertype<ray::NPatchInfo>(
			"rayNPatchInfo",
			sol::no_constructor,
			"source",
			&ray::NPatchInfo::source,
			"left",
			&ray::NPatchInfo::left,
			"top",
			&ray::NPatchInfo::top,
			"right",
			&ray::NPatchInfo::right,
			"bottom",
			&ray::NPatchInfo::bottom,
			"layout",
			&ray::NPatchInfo::layout
		);
		lua.new_usertype<ray::GlyphInfo>(
			"rayGlyphInfo",
			sol::no_constructor,
			"value",
			&ray::GlyphInfo::value,
			"offsetX",
			&ray::GlyphInfo::offsetX,
			"offsetY",
			&ray::GlyphInfo::offsetY,
			"advanceX",
			&ray::GlyphInfo::advanceX,
			"image",
			&ray::GlyphInfo::image
		);
		lua.new_usertype<ray::Font>(
			"rayFont",
			sol::no_constructor,
			"baseSize",
			&ray::Font::baseSize,
			"glyphCount",
			&ray::Font::glyphCount,
			"glyphPadding",
			&ray::Font::glyphPadding,
			"texture",
			&ray::Font::texture,
			"recs",
			&ray::Font::recs,
			"glyphs",
			&ray::Font::glyphs
		);
		lua.new_usertype<ray::Camera3D>(
			"rayCamera",
			sol::call_constructor,
			sol::factories(
				[]() {
					return ray::Camera3D {};
				},
				[](ray::Vector3 position, ray::Vector3 target, ray::Vector3 up, float fovy, int type) {
					return ray::Camera3D {position, target, up, fovy, type};
				},
				[](ray::Vector3 position, ray::Vector3 target, ray::Vector3 up, float fovy) {
					return ray::Camera3D {position, target, up, fovy, 0};
				},
				[](ray::Vector3 position, ray::Vector3 target, ray::Vector3 up) {
					return ray::Camera3D {position, target, up, 0, 0};
				}
			),
			"position",
			&ray::Camera3D::position,
			"target",
			&ray::Camera3D::target,
			"up",
			&ray::Camera3D::up,
			"fovy",
			&ray::Camera3D::fovy,
			"projection",
			&ray::Camera3D::projection
		);
		lua.new_usertype<ray::Camera2D>(
			"rayCamera2D",
			sol::call_constructor,
			sol::factories(
				[]() {
					return ray::Camera2D {};
				},
				[](ray::Vector2 offset, ray::Vector2 target, float rotation, float zoom) {
					return ray::Camera2D {offset, target, rotation, zoom};
				},
				[](ray::Vector2 offset, ray::Vector2 target) {
					return ray::Camera2D {offset, target, 0, 0};
				}
			),
			"offset",
			&ray::Camera2D::offset,
			"target",
			&ray::Camera2D::target,
			"rotation",
			&ray::Camera2D::rotation,
			"zoom",
			&ray::Camera2D::zoom
		);
		lua.new_usertype<ray::Mesh>(
			"rayMesh",
			sol::no_constructor,
			"vertexCount",
			&ray::Mesh::vertexCount,
			"triangleCount",
			&ray::Mesh::triangleCount,
			"vertices",
			&ray::Mesh::vertices,
			"texcoords",
			&ray::Mesh::texcoords,
			"texcoords2",
			&ray::Mesh::texcoords2,
			"normals",
			&ray::Mesh::normals,
			"tangents",
			&ray::Mesh::tangents,
			"colors",
			&ray::Mesh::colors,
			"indices",
			&ray::Mesh::indices,
			"boneCount",
			&ray::Mesh::boneCount,
			"boneIndices",
			&ray::Mesh::boneIndices,
			"boneWeights",
			&ray::Mesh::boneWeights,
			"animVertices",
			&ray::Mesh::animVertices,
			"animNormals",
			&ray::Mesh::animNormals,
			"vaoId",
			&ray::Mesh::vaoId,
			"vboId",
			&ray::Mesh::vboId
		);
		lua.new_usertype<ray::Shader>("rayShader", sol::no_constructor, "id", &ray::Shader::id, "locs", &ray::Shader::locs);
		lua.new_usertype<
			ray::MaterialMap>("rayMaterialMap", sol::no_constructor, "texture", &ray::MaterialMap::texture, "color", &ray::MaterialMap::color, "value", &ray::MaterialMap::value);
		lua.new_usertype<ray::Material>("rayMaterial", sol::no_constructor, "shader", &ray::Material::shader, "maps", &ray::Material::maps, "params", &ray::Material::params);
		lua.new_usertype<ray::Transform>(
			"rayTransform",
			sol::no_constructor,
			"translation",
			&ray::Transform::translation,
			"rotation",
			&ray::Transform::rotation,
			"scale",
			&ray::Transform::scale
		);
		lua.new_usertype<ray::BoneInfo>("rayBoneInfo", sol::no_constructor, "name", &ray::BoneInfo::name, "parent", &ray::BoneInfo::parent);
		lua.new_usertype<ray::ModelSkeleton>(
			"rayModelSkeleton",
			sol::no_constructor,
			"boneCount",
			&ray::ModelSkeleton::boneCount,
			"bones",
			&ray::ModelSkeleton::bones,
			"bindPose",
			&ray::ModelSkeleton::bindPose
		);

		lua.new_usertype<ray::Model>(
			"rayModel",
			sol::no_constructor,
			"transform",
			&ray::Model::transform,
			"meshCount",
			&ray::Model::meshCount,
			"meshes",
			&ray::Model::meshes,
			"materialCount",
			&ray::Model::materialCount,
			"materials",
			&ray::Model::materials,
			"meshMaterial",
			&ray::Model::meshMaterial,
			"skeleton",
			&ray::Model::skeleton,
			"currentPose",
			&ray::Model::currentPose,
			"boneMatrices",
			&ray::Model::boneMatrices
		);
		lua.new_usertype<ray::ModelAnimation>(
			"rayModelAnimation",
			sol::no_constructor,
			"name",
			&ray::ModelAnimation::name,
			"boneCount",
			&ray::ModelAnimation::boneCount,
			"keyframeCount",
			&ray::ModelAnimation::keyframeCount
			//"keyframePoses",
			//&ray::ModelAnimation::keyframePoses
		);
		lua.new_usertype<ray::Ray>(
			"rayRay",
			sol::call_constructor,
			sol::factories(
				[]() {
					return ray::Ray {};
				},
				[](ray::Vector3 pos) {
					return ray::Ray {pos};
				},
				[](ray::Vector3 pos, ray::Vector3 dir) {
					return ray::Ray {pos, dir};
				}
			),
			"position",
			&ray::Ray::position,
			"direction",
			&ray::Ray::direction
		);
		lua.new_usertype<ray::RayCollision>(
			"rayRayCollision",
			sol::no_constructor,
			"hit",
			&ray::RayCollision::hit,
			"distance",
			&ray::RayCollision::distance,
			"point",
			&ray::RayCollision::point,
			"normal",
			&ray::RayCollision::normal
		);
		lua.new_usertype<ray::BoundingBox>(
			"rayBoundingBox",
			sol::call_constructor,
			sol::factories(
				[]() {
					return ray::BoundingBox {};
				},
				[](ray::Vector3 minimum, ray::Vector3 maximum) {
					return ray::BoundingBox {minimum, maximum};
				}
			),
			"min",
			&ray::BoundingBox::min,
			"max",
			&ray::BoundingBox::max
		);
		lua.new_usertype<ray::Wave>(
			"rayWave",
			sol::no_constructor,
			"frameCount",
			&ray::Wave::frameCount,
			"sampleRate",
			&ray::Wave::sampleRate,
			"sampleSize",
			&ray::Wave::sampleSize,
			"channels",
			&ray::Wave::channels,
			"data",
			&ray::Wave::data
		);
		lua.new_usertype<ray::AudioStream>(
			"rayAudioStream",
			sol::no_constructor,
			//"buffer",
			//&ray::AudioStream::buffer,
			//"processor",
			//&ray::AudioStream::processor,
			"sampleRate",
			&ray::AudioStream::sampleRate,
			"sampleSize",
			&ray::AudioStream::sampleSize,
			"channels",
			&ray::AudioStream::channels
		);
		lua.new_usertype<ray::Sound>("raySound", sol::no_constructor, "stream", &ray::Sound::stream, "frameCount", &ray::Sound::frameCount);
		lua.new_usertype<ray::Music>(
			"rayMusic",
			sol::no_constructor,
			"stream",
			&ray::Music::stream,
			"frameCount",
			&ray::Music::frameCount,
			"looping",
			&ray::Music::looping,
			"ctxType",
			&ray::Music::ctxType,
			"ctxData",
			&ray::Music::ctxData
		);
		lua.new_usertype<ray::FilePathList>("rayFilePathList", sol::no_constructor, "count", &ray::FilePathList::count, "paths", &ray::FilePathList::paths);
		lua.new_usertype<ray::AutomationEvent>(
			"rayAutomationEvent",
			sol::no_constructor,
			"frame",
			&ray::AutomationEvent::frame,
			"type",
			&ray::AutomationEvent::type,
			"params",
			&ray::AutomationEvent::params
		);
		lua.new_usertype<ray::AutomationEventList>(
			"rayAutomationEventList",
			sol::no_constructor,
			"capacity",
			&ray::AutomationEventList::capacity,
			"count",
			&ray::AutomationEventList::count,
			"events",
			&ray::AutomationEventList::events
		);
		lua.new_usertype<ray::float3>(
			"rayFloat3",
			sol::call_constructor,
			sol::factories(
				[]() {
					return ray::float3 {};
				},
				[](float x, float y, float z) {
					return ray::float3 {
						.v = {x, y, z}
					};
				}
			),
			"v",
			&ray::float3::v
		);
		lua.new_usertype<ray::float16>(
			"rayFloat16",
			sol::call_constructor,
			sol::factories([]() {
				return ray::float16 {};
			}),
			"v",
			&ray::float16::v
		);

		// VrDeviceInfo
		// VrStereoConfig

		// clang-format off
		lua.new_enum<ray::ConfigFlags>("rayConfigFlags", {
			{"RAY_FLAG_VSYNC_HINT", ray::ConfigFlags::FLAG_VSYNC_HINT},
			{"RAY_FLAG_FULLSCREEN_MODE", ray::ConfigFlags::FLAG_FULLSCREEN_MODE},
			{"RAY_FLAG_WINDOW_RESIZABLE", ray::ConfigFlags::FLAG_WINDOW_RESIZABLE},
			{"RAY_FLAG_WINDOW_UNDECORATED", ray::ConfigFlags::FLAG_WINDOW_UNDECORATED},
			{"RAY_FLAG_WINDOW_HIDDEN", ray::ConfigFlags::FLAG_WINDOW_HIDDEN},
			{"RAY_FLAG_WINDOW_MINIMIZED", ray::ConfigFlags::FLAG_WINDOW_MINIMIZED},
			{"RAY_FLAG_WINDOW_MAXIMIZED", ray::ConfigFlags::FLAG_WINDOW_MAXIMIZED},
			{"RAY_FLAG_WINDOW_UNFOCUSED", ray::ConfigFlags::FLAG_WINDOW_UNFOCUSED},
			{"RAY_FLAG_WINDOW_TOPMOST", ray::ConfigFlags::FLAG_WINDOW_TOPMOST},
			{"RAY_FLAG_WINDOW_ALWAYS_RUN", ray::ConfigFlags::FLAG_WINDOW_ALWAYS_RUN},
			{"RAY_FLAG_WINDOW_TRANSPARENT", ray::ConfigFlags::FLAG_WINDOW_TRANSPARENT},
			{"RAY_FLAG_WINDOW_HIGHDPI", ray::ConfigFlags::FLAG_WINDOW_HIGHDPI},
			{"RAY_FLAG_WINDOW_MOUSE_PASSTHROUGH", ray::ConfigFlags::FLAG_WINDOW_MOUSE_PASSTHROUGH},
			{"RAY_FLAG_BORDERLESS_WINDOWED_MODE", ray::ConfigFlags::FLAG_BORDERLESS_WINDOWED_MODE},
			{"RAY_FLAG_MSAA_4X_HINT", ray::ConfigFlags::FLAG_MSAA_4X_HINT},
			{"RAY_FLAG_INTERLACED_HINT", ray::ConfigFlags::FLAG_INTERLACED_HINT}
		});
		lua.new_enum<ray::TraceLogLevel>("rayTraceLogLevel", {
			{"RAY_LOG_ALL", ray::TraceLogLevel::LOG_ALL},
			{"RAY_LOG_TRACE", ray::TraceLogLevel::LOG_TRACE},
			{"RAY_LOG_DEBUG", ray::TraceLogLevel::LOG_DEBUG},
			{"RAY_LOG_INFO", ray::TraceLogLevel::LOG_INFO},
			{"RAY_LOG_WARNING", ray::TraceLogLevel::LOG_WARNING},
			{"RAY_LOG_ERROR", ray::TraceLogLevel::LOG_ERROR},
			{"RAY_LOG_FATAL", ray::TraceLogLevel::LOG_FATAL},
			{"RAY_LOG_NONE", ray::TraceLogLevel::LOG_NONE}
		});
		lua.new_enum<ray::KeyboardKey>("rayKeyboardKey", {
			{"RAY_KEY_NULL", ray::KeyboardKey::KEY_NULL},
			{"RAY_KEY_APOSTROPHE", ray::KeyboardKey::KEY_APOSTROPHE},
			{"RAY_KEY_COMMA", ray::KeyboardKey::KEY_COMMA},
			{"RAY_KEY_MINUS", ray::KeyboardKey::KEY_MINUS},
			{"RAY_KEY_PERIOD", ray::KeyboardKey::KEY_PERIOD},
			{"RAY_KEY_SLASH", ray::KeyboardKey::KEY_SLASH},
			{"RAY_KEY_ZERO", ray::KeyboardKey::KEY_ZERO},
			{"RAY_KEY_ONE", ray::KeyboardKey::KEY_ONE},
			{"RAY_KEY_TWO", ray::KeyboardKey::KEY_TWO},
			{"RAY_KEY_THREE", ray::KeyboardKey::KEY_THREE},
			{"RAY_KEY_FOUR", ray::KeyboardKey::KEY_FOUR},
			{"RAY_KEY_FIVE", ray::KeyboardKey::KEY_FIVE},
			{"RAY_KEY_SIX", ray::KeyboardKey::KEY_SIX},
			{"RAY_KEY_SEVEN", ray::KeyboardKey::KEY_SEVEN},
			{"RAY_KEY_EIGHT", ray::KeyboardKey::KEY_EIGHT},
			{"RAY_KEY_NINE", ray::KeyboardKey::KEY_NINE},
			{"RAY_KEY_SEMICOLON", ray::KeyboardKey::KEY_SEMICOLON},
			{"RAY_KEY_EQUAL", ray::KeyboardKey::KEY_EQUAL},
			{"RAY_KEY_A", ray::KeyboardKey::KEY_A},
			{"RAY_KEY_B", ray::KeyboardKey::KEY_B},
			{"RAY_KEY_C", ray::KeyboardKey::KEY_C},
			{"RAY_KEY_D", ray::KeyboardKey::KEY_D},
			{"RAY_KEY_E", ray::KeyboardKey::KEY_E},
			{"RAY_KEY_F", ray::KeyboardKey::KEY_F},
			{"RAY_KEY_G", ray::KeyboardKey::KEY_G},
			{"RAY_KEY_H", ray::KeyboardKey::KEY_H},
			{"RAY_KEY_I", ray::KeyboardKey::KEY_I},
			{"RAY_KEY_J", ray::KeyboardKey::KEY_J},
			{"RAY_KEY_K", ray::KeyboardKey::KEY_K},
			{"RAY_KEY_L", ray::KeyboardKey::KEY_L},
			{"RAY_KEY_M", ray::KeyboardKey::KEY_M},
			{"RAY_KEY_N", ray::KeyboardKey::KEY_N},
			{"RAY_KEY_O", ray::KeyboardKey::KEY_O},
			{"RAY_KEY_P", ray::KeyboardKey::KEY_P},
			{"RAY_KEY_Q", ray::KeyboardKey::KEY_Q},
			{"RAY_KEY_R", ray::KeyboardKey::KEY_R},
			{"RAY_KEY_S", ray::KeyboardKey::KEY_S},
			{"RAY_KEY_T", ray::KeyboardKey::KEY_T},
			{"RAY_KEY_U", ray::KeyboardKey::KEY_U},
			{"RAY_KEY_V", ray::KeyboardKey::KEY_V},
			{"RAY_KEY_W", ray::KeyboardKey::KEY_W},
			{"RAY_KEY_X", ray::KeyboardKey::KEY_X},
			{"RAY_KEY_Y", ray::KeyboardKey::KEY_Y},
			{"RAY_KEY_Z", ray::KeyboardKey::KEY_Z},
			{"RAY_KEY_LEFT_BRACKET", ray::KeyboardKey::KEY_LEFT_BRACKET},
			{"RAY_KEY_BACKSLASH", ray::KeyboardKey::KEY_BACKSLASH},
			{"RAY_KEY_RIGHT_BRACKET", ray::KeyboardKey::KEY_RIGHT_BRACKET},
			{"RAY_KEY_GRAVE", ray::KeyboardKey::KEY_GRAVE},
			{"RAY_KEY_SPACE", ray::KeyboardKey::KEY_SPACE},
			{"RAY_KEY_ESCAPE", ray::KeyboardKey::KEY_ESCAPE},
			{"RAY_KEY_ENTER", ray::KeyboardKey::KEY_ENTER},
			{"RAY_KEY_TAB", ray::KeyboardKey::KEY_TAB},
			{"RAY_KEY_BACKSPACE", ray::KeyboardKey::KEY_BACKSPACE},
			{"RAY_KEY_INSERT", ray::KeyboardKey::KEY_INSERT},
			{"RAY_KEY_DELETE", ray::KeyboardKey::KEY_DELETE},
			{"RAY_KEY_RIGHT", ray::KeyboardKey::KEY_RIGHT},
			{"RAY_KEY_LEFT", ray::KeyboardKey::KEY_LEFT},
			{"RAY_KEY_DOWN", ray::KeyboardKey::KEY_DOWN},
			{"RAY_KEY_UP", ray::KeyboardKey::KEY_UP},
			{"RAY_KEY_PAGE_UP", ray::KeyboardKey::KEY_PAGE_UP},
			{"RAY_KEY_PAGE_DOWN", ray::KeyboardKey::KEY_PAGE_DOWN},
			{"RAY_KEY_HOME", ray::KeyboardKey::KEY_HOME},
			{"RAY_KEY_END", ray::KeyboardKey::KEY_END},
			{"RAY_KEY_CAPS_LOCK", ray::KeyboardKey::KEY_CAPS_LOCK},
			{"RAY_KEY_SCROLL_LOCK", ray::KeyboardKey::KEY_SCROLL_LOCK},
			{"RAY_KEY_NUM_LOCK", ray::KeyboardKey::KEY_NUM_LOCK},
			{"RAY_KEY_PRINT_SCREEN", ray::KeyboardKey::KEY_PRINT_SCREEN},
			{"RAY_KEY_PAUSE", ray::KeyboardKey::KEY_PAUSE},
			{"RAY_KEY_F1", ray::KeyboardKey::KEY_F1},
			{"RAY_KEY_F2", ray::KeyboardKey::KEY_F2},
			{"RAY_KEY_F3", ray::KeyboardKey::KEY_F3},
			{"RAY_KEY_F4", ray::KeyboardKey::KEY_F4},
			{"RAY_KEY_F5", ray::KeyboardKey::KEY_F5},
			{"RAY_KEY_F6", ray::KeyboardKey::KEY_F6},
			{"RAY_KEY_F7", ray::KeyboardKey::KEY_F7},
			{"RAY_KEY_F8", ray::KeyboardKey::KEY_F8},
			{"RAY_KEY_F9", ray::KeyboardKey::KEY_F9},
			{"RAY_KEY_F10", ray::KeyboardKey::KEY_F10},
			{"RAY_KEY_F11", ray::KeyboardKey::KEY_F11},
			{"RAY_KEY_F12", ray::KeyboardKey::KEY_F12},
			{"RAY_KEY_LEFT_SHIFT", ray::KeyboardKey::KEY_LEFT_SHIFT},
			{"RAY_KEY_LEFT_CONTROL", ray::KeyboardKey::KEY_LEFT_CONTROL},
			{"RAY_KEY_LEFT_ALT", ray::KeyboardKey::KEY_LEFT_ALT},
			{"RAY_KEY_LEFT_SUPER", ray::KeyboardKey::KEY_LEFT_SUPER},
			{"RAY_KEY_RIGHT_SHIFT", ray::KeyboardKey::KEY_RIGHT_SHIFT},
			{"RAY_KEY_RIGHT_CONTROL", ray::KeyboardKey::KEY_RIGHT_CONTROL},
			{"RAY_KEY_RIGHT_ALT", ray::KeyboardKey::KEY_RIGHT_ALT},
			{"RAY_KEY_RIGHT_SUPER", ray::KeyboardKey::KEY_RIGHT_SUPER},
			{"RAY_KEY_KB_MENU", ray::KeyboardKey::KEY_KB_MENU},
			{"RAY_KEY_KP_0", ray::KeyboardKey::KEY_KP_0},
			{"RAY_KEY_KP_1", ray::KeyboardKey::KEY_KP_1},
			{"RAY_KEY_KP_2", ray::KeyboardKey::KEY_KP_2},
			{"RAY_KEY_KP_3", ray::KeyboardKey::KEY_KP_3},
			{"RAY_KEY_KP_4", ray::KeyboardKey::KEY_KP_4},
			{"RAY_KEY_KP_5", ray::KeyboardKey::KEY_KP_5},
			{"RAY_KEY_KP_6", ray::KeyboardKey::KEY_KP_6},
			{"RAY_KEY_KP_7", ray::KeyboardKey::KEY_KP_7},
			{"RAY_KEY_KP_8", ray::KeyboardKey::KEY_KP_8},
			{"RAY_KEY_KP_9", ray::KeyboardKey::KEY_KP_9},
			{"RAY_KEY_KP_DECIMAL", ray::KeyboardKey::KEY_KP_DECIMAL},
			{"RAY_KEY_KP_DIVIDE", ray::KeyboardKey::KEY_KP_DIVIDE},
			{"RAY_KEY_KP_MULTIPLY", ray::KeyboardKey::KEY_KP_MULTIPLY},
			{"RAY_KEY_KP_SUBTRACT", ray::KeyboardKey::KEY_KP_SUBTRACT},
			{"RAY_KEY_KP_ADD", ray::KeyboardKey::KEY_KP_ADD},
			{"RAY_KEY_KP_ENTER", ray::KeyboardKey::KEY_KP_ENTER},
			{"RAY_KEY_KP_EQUAL", ray::KeyboardKey::KEY_KP_EQUAL},
			{"RAY_KEY_BACK", ray::KeyboardKey::KEY_BACK},
			{"RAY_KEY_MENU", ray::KeyboardKey::KEY_MENU},
			{"RAY_KEY_VOLUME_UP", ray::KeyboardKey::KEY_VOLUME_UP},
			{"RAY_KEY_VOLUME_DOWN", ray::KeyboardKey::KEY_VOLUME_DOWN}
		});
		lua.new_enum<ray::MouseButton>("rayMouseButton", {
			{"RAY_MOUSE_BUTTON_LEFT", ray::MouseButton::MOUSE_BUTTON_LEFT},
			{"RAY_MOUSE_BUTTON_RIGHT", ray::MouseButton::MOUSE_BUTTON_RIGHT},
			{"RAY_MOUSE_BUTTON_MIDDLE", ray::MouseButton::MOUSE_BUTTON_MIDDLE},
			{"RAY_MOUSE_BUTTON_SIDE", ray::MouseButton::MOUSE_BUTTON_SIDE},
			{"RAY_MOUSE_BUTTON_EXTRA", ray::MouseButton::MOUSE_BUTTON_EXTRA},
			{"RAY_MOUSE_BUTTON_FORWARD", ray::MouseButton::MOUSE_BUTTON_FORWARD},
			{"RAY_MOUSE_BUTTON_BACK", ray::MouseButton::MOUSE_BUTTON_BACK}
		});
		lua.new_enum<ray::MouseCursor>("rayMouseCursor", {
			{"RAY_MOUSE_CURSOR_DEFAULT", ray::MouseCursor::MOUSE_CURSOR_DEFAULT},
			{"RAY_MOUSE_CURSOR_ARROW", ray::MouseCursor::MOUSE_CURSOR_ARROW},
			{"RAY_MOUSE_CURSOR_IBEAM", ray::MouseCursor::MOUSE_CURSOR_IBEAM},
			{"RAY_MOUSE_CURSOR_CROSSHAIR", ray::MouseCursor::MOUSE_CURSOR_CROSSHAIR},
			{"RAY_MOUSE_CURSOR_POINTING_HAND", ray::MouseCursor::MOUSE_CURSOR_POINTING_HAND},
			{"RAY_MOUSE_CURSOR_RESIZE_EW", ray::MouseCursor::MOUSE_CURSOR_RESIZE_EW},
			{"RAY_MOUSE_CURSOR_RESIZE_NS", ray::MouseCursor::MOUSE_CURSOR_RESIZE_NS},
			{"RAY_MOUSE_CURSOR_RESIZE_NWSE", ray::MouseCursor::MOUSE_CURSOR_RESIZE_NWSE},
			{"RAY_MOUSE_CURSOR_RESIZE_NESW", ray::MouseCursor::MOUSE_CURSOR_RESIZE_NESW},
			{"RAY_MOUSE_CURSOR_RESIZE_ALL", ray::MouseCursor::MOUSE_CURSOR_RESIZE_ALL},
			{"RAY_MOUSE_CURSOR_NOT_ALLOWED", ray::MouseCursor::MOUSE_CURSOR_NOT_ALLOWED}
		});
		lua.new_enum<ray::GamepadButton>("rayGamepadButton", {
			{"RAY_GAMEPAD_BUTTON_UNKNOWN", ray::GamepadButton::GAMEPAD_BUTTON_UNKNOWN},
			{"RAY_GAMEPAD_BUTTON_LEFT_FACE_UP", ray::GamepadButton::GAMEPAD_BUTTON_LEFT_FACE_UP},
			{"RAY_GAMEPAD_BUTTON_LEFT_FACE_RIGHT", ray::GamepadButton::GAMEPAD_BUTTON_LEFT_FACE_RIGHT},
			{"RAY_GAMEPAD_BUTTON_LEFT_FACE_DOWN", ray::GamepadButton::GAMEPAD_BUTTON_LEFT_FACE_DOWN},
			{"RAY_GAMEPAD_BUTTON_LEFT_FACE_LEFT", ray::GamepadButton::GAMEPAD_BUTTON_LEFT_FACE_LEFT},
			{"RAY_GAMEPAD_BUTTON_RIGHT_FACE_UP", ray::GamepadButton::GAMEPAD_BUTTON_RIGHT_FACE_UP},
			{"RAY_GAMEPAD_BUTTON_RIGHT_FACE_RIGHT", ray::GamepadButton::GAMEPAD_BUTTON_RIGHT_FACE_RIGHT},
			{"RAY_GAMEPAD_BUTTON_RIGHT_FACE_DOWN", ray::GamepadButton::GAMEPAD_BUTTON_RIGHT_FACE_DOWN},
			{"RAY_GAMEPAD_BUTTON_RIGHT_FACE_LEFT", ray::GamepadButton::GAMEPAD_BUTTON_RIGHT_FACE_LEFT},
			{"RAY_GAMEPAD_BUTTON_LEFT_TRIGGER_1", ray::GamepadButton::GAMEPAD_BUTTON_LEFT_TRIGGER_1},
			{"RAY_GAMEPAD_BUTTON_LEFT_TRIGGER_2", ray::GamepadButton::GAMEPAD_BUTTON_LEFT_TRIGGER_2},
			{"RAY_GAMEPAD_BUTTON_RIGHT_TRIGGER_1", ray::GamepadButton::GAMEPAD_BUTTON_RIGHT_TRIGGER_1},
			{"RAY_GAMEPAD_BUTTON_RIGHT_TRIGGER_2", ray::GamepadButton::GAMEPAD_BUTTON_RIGHT_TRIGGER_2},
			{"RAY_GAMEPAD_BUTTON_MIDDLE_LEFT", ray::GamepadButton::GAMEPAD_BUTTON_MIDDLE_LEFT},
			{"RAY_GAMEPAD_BUTTON_MIDDLE", ray::GamepadButton::GAMEPAD_BUTTON_MIDDLE},
			{"RAY_GAMEPAD_BUTTON_MIDDLE_RIGHT", ray::GamepadButton::GAMEPAD_BUTTON_MIDDLE_RIGHT},
			{"RAY_GAMEPAD_BUTTON_LEFT_THUMB", ray::GamepadButton::GAMEPAD_BUTTON_LEFT_THUMB},
			{"RAY_GAMEPAD_BUTTON_RIGHT_THUMB", ray::GamepadButton::GAMEPAD_BUTTON_RIGHT_THUMB}
		});
		lua.new_enum<ray::GamepadAxis>("rayGamepadAxis", {
			{"RAY_GAMEPAD_AXIS_LEFT_X", ray::GamepadAxis::GAMEPAD_AXIS_LEFT_X},
			{"RAY_GAMEPAD_AXIS_LEFT_Y", ray::GamepadAxis::GAMEPAD_AXIS_LEFT_Y},
			{"RAY_GAMEPAD_AXIS_RIGHT_X", ray::GamepadAxis::GAMEPAD_AXIS_RIGHT_X},
			{"RAY_GAMEPAD_AXIS_RIGHT_Y", ray::GamepadAxis::GAMEPAD_AXIS_RIGHT_Y},
			{"RAY_GAMEPAD_AXIS_LEFT_TRIGGER", ray::GamepadAxis::GAMEPAD_AXIS_LEFT_TRIGGER},
			{"RAY_GAMEPAD_AXIS_RIGHT_TRIGGER", ray::GamepadAxis::GAMEPAD_AXIS_RIGHT_TRIGGER}
		});
		lua.new_enum<ray::MaterialMapIndex>("rayMaterialMapIndex", {
			{"RAY_MATERIAL_MAP_ALBEDO", ray::MaterialMapIndex::MATERIAL_MAP_ALBEDO},
			{"RAY_MATERIAL_MAP_METALNESS", ray::MaterialMapIndex::MATERIAL_MAP_METALNESS},
			{"RAY_MATERIAL_MAP_NORMAL", ray::MaterialMapIndex::MATERIAL_MAP_NORMAL},
			{"RAY_MATERIAL_MAP_ROUGHNESS", ray::MaterialMapIndex::MATERIAL_MAP_ROUGHNESS},
			{"RAY_MATERIAL_MAP_OCCLUSION", ray::MaterialMapIndex::MATERIAL_MAP_OCCLUSION},
			{"RAY_MATERIAL_MAP_EMISSION", ray::MaterialMapIndex::MATERIAL_MAP_EMISSION},
			{"RAY_MATERIAL_MAP_HEIGHT", ray::MaterialMapIndex::MATERIAL_MAP_HEIGHT},
			{"RAY_MATERIAL_MAP_CUBEMAP", ray::MaterialMapIndex::MATERIAL_MAP_CUBEMAP},
			{"RAY_MATERIAL_MAP_IRRADIANCE", ray::MaterialMapIndex::MATERIAL_MAP_IRRADIANCE},
			{"RAY_MATERIAL_MAP_PREFILTER", ray::MaterialMapIndex::MATERIAL_MAP_PREFILTER},
			{"RAY_MATERIAL_MAP_BRDF", ray::MaterialMapIndex::MATERIAL_MAP_BRDF}
		});
		lua.new_enum<ray::ShaderLocationIndex>("rayShaderLocationIndex", {
			{"RAY_SHADER_LOC_VERTEX_POSITION", ray::ShaderLocationIndex::SHADER_LOC_VERTEX_POSITION},
			{"RAY_SHADER_LOC_VERTEX_TEXCOORD01", ray::ShaderLocationIndex::SHADER_LOC_VERTEX_TEXCOORD01},
			{"RAY_SHADER_LOC_VERTEX_TEXCOORD02", ray::ShaderLocationIndex::SHADER_LOC_VERTEX_TEXCOORD02},
			{"RAY_SHADER_LOC_VERTEX_NORMAL", ray::ShaderLocationIndex::SHADER_LOC_VERTEX_NORMAL},
			{"RAY_SHADER_LOC_VERTEX_TANGENT", ray::ShaderLocationIndex::SHADER_LOC_VERTEX_TANGENT},
			{"RAY_SHADER_LOC_VERTEX_COLOR", ray::ShaderLocationIndex::SHADER_LOC_VERTEX_COLOR},
			{"RAY_SHADER_LOC_MATRIX_MVP", ray::ShaderLocationIndex::SHADER_LOC_MATRIX_MVP},
			{"RAY_SHADER_LOC_MATRIX_VIEW", ray::ShaderLocationIndex::SHADER_LOC_MATRIX_VIEW},
			{"RAY_SHADER_LOC_MATRIX_PROJECTION", ray::ShaderLocationIndex::SHADER_LOC_MATRIX_PROJECTION},
			{"RAY_SHADER_LOC_MATRIX_MODEL", ray::ShaderLocationIndex::SHADER_LOC_MATRIX_MODEL},
			{"RAY_SHADER_LOC_MATRIX_NORMAL", ray::ShaderLocationIndex::SHADER_LOC_MATRIX_NORMAL},
			{"RAY_SHADER_LOC_VECTOR_VIEW", ray::ShaderLocationIndex::SHADER_LOC_VECTOR_VIEW},
			{"RAY_SHADER_LOC_COLOR_DIFFUSE", ray::ShaderLocationIndex::SHADER_LOC_COLOR_DIFFUSE},
			{"RAY_SHADER_LOC_COLOR_SPECULAR", ray::ShaderLocationIndex::SHADER_LOC_COLOR_SPECULAR},
			{"RAY_SHADER_LOC_COLOR_AMBIENT", ray::ShaderLocationIndex::SHADER_LOC_COLOR_AMBIENT},
			{"RAY_SHADER_LOC_MAP_ALBEDO", ray::ShaderLocationIndex::SHADER_LOC_MAP_ALBEDO},
			{"RAY_SHADER_LOC_MAP_METALNESS", ray::ShaderLocationIndex::SHADER_LOC_MAP_METALNESS},
			{"RAY_SHADER_LOC_MAP_NORMAL", ray::ShaderLocationIndex::SHADER_LOC_MAP_NORMAL},
			{"RAY_SHADER_LOC_MAP_ROUGHNESS", ray::ShaderLocationIndex::SHADER_LOC_MAP_ROUGHNESS},
			{"RAY_SHADER_LOC_MAP_OCCLUSION", ray::ShaderLocationIndex::SHADER_LOC_MAP_OCCLUSION},
			{"RAY_SHADER_LOC_MAP_EMISSION", ray::ShaderLocationIndex::SHADER_LOC_MAP_EMISSION},
			{"RAY_SHADER_LOC_MAP_HEIGHT", ray::ShaderLocationIndex::SHADER_LOC_MAP_HEIGHT},
			{"RAY_SHADER_LOC_MAP_CUBEMAP", ray::ShaderLocationIndex::SHADER_LOC_MAP_CUBEMAP},
			{"RAY_SHADER_LOC_MAP_IRRADIANCE", ray::ShaderLocationIndex::SHADER_LOC_MAP_IRRADIANCE},
			{"RAY_SHADER_LOC_MAP_PREFILTER", ray::ShaderLocationIndex::SHADER_LOC_MAP_PREFILTER},
			{"RAY_SHADER_LOC_MAP_BRDF", ray::ShaderLocationIndex::SHADER_LOC_MAP_BRDF},
			{"RAY_SHADER_LOC_VERTEX_BONEIDS", ray::ShaderLocationIndex::SHADER_LOC_VERTEX_BONEIDS},
			{"RAY_SHADER_LOC_VERTEX_BONEWEIGHTS", ray::ShaderLocationIndex::SHADER_LOC_VERTEX_BONEWEIGHTS},
			{"RAY_SHADER_LOC_MATRIX_BONETRANSFORMS", ray::ShaderLocationIndex::SHADER_LOC_MATRIX_BONETRANSFORMS},
			{"RAY_SHADER_LOC_VERTEX_INSTANCETRANSFORM", ray::ShaderLocationIndex::SHADER_LOC_VERTEX_INSTANCETRANSFORM}
		});
		lua.new_enum<ray::ShaderUniformDataType>("rayShaderUniformDataType", {
			{"RAY_SHADER_UNIFORM_FLOAT", ray::ShaderUniformDataType::SHADER_UNIFORM_FLOAT},
			{"RAY_SHADER_UNIFORM_VEC2", ray::ShaderUniformDataType::SHADER_UNIFORM_VEC2},
			{"RAY_SHADER_UNIFORM_VEC3", ray::ShaderUniformDataType::SHADER_UNIFORM_VEC3},
			{"RAY_SHADER_UNIFORM_VEC4", ray::ShaderUniformDataType::SHADER_UNIFORM_VEC4},
			{"RAY_SHADER_UNIFORM_INT", ray::ShaderUniformDataType::SHADER_UNIFORM_INT},
			{"RAY_SHADER_UNIFORM_IVEC2", ray::ShaderUniformDataType::SHADER_UNIFORM_IVEC2},
			{"RAY_SHADER_UNIFORM_IVEC3", ray::ShaderUniformDataType::SHADER_UNIFORM_IVEC3},
			{"RAY_SHADER_UNIFORM_IVEC4", ray::ShaderUniformDataType::SHADER_UNIFORM_IVEC4},
			{"RAY_SHADER_UNIFORM_UINT", ray::ShaderUniformDataType::SHADER_UNIFORM_UINT},
			{"RAY_SHADER_UNIFORM_UIVEC2", ray::ShaderUniformDataType::SHADER_UNIFORM_UIVEC2},
			{"RAY_SHADER_UNIFORM_UIVEC3", ray::ShaderUniformDataType::SHADER_UNIFORM_UIVEC3},
			{"RAY_SHADER_UNIFORM_UIVEC4", ray::ShaderUniformDataType::SHADER_UNIFORM_UIVEC4},
			{"RAY_SHADER_UNIFORM_SAMPLER2D", ray::ShaderUniformDataType::SHADER_UNIFORM_SAMPLER2D}
		});
		lua.new_enum<ray::ShaderAttributeDataType>("rayShaderAttributeDataType", {
			{"RAY_SHADER_ATTRIB_FLOAT", ray::ShaderAttributeDataType::SHADER_ATTRIB_FLOAT},
			{"RAY_SHADER_ATTRIB_VEC2", ray::ShaderAttributeDataType::SHADER_ATTRIB_VEC2},
			{"RAY_SHADER_ATTRIB_VEC3", ray::ShaderAttributeDataType::SHADER_ATTRIB_VEC3},
			{"RAY_SHADER_ATTRIB_VEC4", ray::ShaderAttributeDataType::SHADER_ATTRIB_VEC4}
		});
		lua.new_enum<ray::PixelFormat>("rayPixelFormat", {
			{"RAY_PIXELFORMAT_UNCOMPRESSED_GRAYSCALE", ray::PixelFormat::PIXELFORMAT_UNCOMPRESSED_GRAYSCALE},
			{"RAY_PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA", ray::PixelFormat::PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA},
			{"RAY_PIXELFORMAT_UNCOMPRESSED_R5G6B5", ray::PixelFormat::PIXELFORMAT_UNCOMPRESSED_R5G6B5},
			{"RAY_PIXELFORMAT_UNCOMPRESSED_R8G8B8", ray::PixelFormat::PIXELFORMAT_UNCOMPRESSED_R8G8B8},
			{"RAY_PIXELFORMAT_UNCOMPRESSED_R5G5B5A1", ray::PixelFormat::PIXELFORMAT_UNCOMPRESSED_R5G5B5A1},
			{"RAY_PIXELFORMAT_UNCOMPRESSED_R4G4B4A4", ray::PixelFormat::PIXELFORMAT_UNCOMPRESSED_R4G4B4A4},
			{"RAY_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8", ray::PixelFormat::PIXELFORMAT_UNCOMPRESSED_R8G8B8A8},
			{"RAY_PIXELFORMAT_UNCOMPRESSED_R32", ray::PixelFormat::PIXELFORMAT_UNCOMPRESSED_R32},
			{"RAY_PIXELFORMAT_UNCOMPRESSED_R32G32B32", ray::PixelFormat::PIXELFORMAT_UNCOMPRESSED_R32G32B32},
			{"RAY_PIXELFORMAT_UNCOMPRESSED_R32G32B32A32", ray::PixelFormat::PIXELFORMAT_UNCOMPRESSED_R32G32B32A32},
			{"RAY_PIXELFORMAT_UNCOMPRESSED_R16", ray::PixelFormat::PIXELFORMAT_UNCOMPRESSED_R16},
			{"RAY_PIXELFORMAT_UNCOMPRESSED_R16G16B16", ray::PixelFormat::PIXELFORMAT_UNCOMPRESSED_R16G16B16},
			{"RAY_PIXELFORMAT_UNCOMPRESSED_R16G16B16A16", ray::PixelFormat::PIXELFORMAT_UNCOMPRESSED_R16G16B16A16},
			{"RAY_PIXELFORMAT_COMPRESSED_DXT1_RGB", ray::PixelFormat::PIXELFORMAT_COMPRESSED_DXT1_RGB},
			{"RAY_PIXELFORMAT_COMPRESSED_DXT1_RGBA", ray::PixelFormat::PIXELFORMAT_COMPRESSED_DXT1_RGBA},
			{"RAY_PIXELFORMAT_COMPRESSED_DXT3_RGBA", ray::PixelFormat::PIXELFORMAT_COMPRESSED_DXT3_RGBA},
			{"RAY_PIXELFORMAT_COMPRESSED_DXT5_RGBA", ray::PixelFormat::PIXELFORMAT_COMPRESSED_DXT5_RGBA},
			{"RAY_PIXELFORMAT_COMPRESSED_ETC1_RGB", ray::PixelFormat::PIXELFORMAT_COMPRESSED_ETC1_RGB},
			{"RAY_PIXELFORMAT_COMPRESSED_ETC2_RGB", ray::PixelFormat::PIXELFORMAT_COMPRESSED_ETC2_RGB},
			{"RAY_PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA", ray::PixelFormat::PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA},
			{"RAY_PIXELFORMAT_COMPRESSED_PVRT_RGB", ray::PixelFormat::PIXELFORMAT_COMPRESSED_PVRT_RGB},
			{"RAY_PIXELFORMAT_COMPRESSED_PVRT_RGBA", ray::PixelFormat::PIXELFORMAT_COMPRESSED_PVRT_RGBA},
			{"RAY_PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA", ray::PixelFormat::PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA},
			{"RAY_PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA", ray::PixelFormat::PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA}
		});
		lua.new_enum<ray::TextureFilter>("rayTextureFilter", {
			{"RAY_TEXTURE_FILTER_POINT", ray::TextureFilter::TEXTURE_FILTER_POINT},
			{"RAY_TEXTURE_FILTER_BILINEAR", ray::TextureFilter::TEXTURE_FILTER_BILINEAR},
			{"RAY_TEXTURE_FILTER_TRILINEAR", ray::TextureFilter::TEXTURE_FILTER_TRILINEAR},
			{"RAY_TEXTURE_FILTER_ANISOTROPIC_4X", ray::TextureFilter::TEXTURE_FILTER_ANISOTROPIC_4X},
			{"RAY_TEXTURE_FILTER_ANISOTROPIC_8X", ray::TextureFilter::TEXTURE_FILTER_ANISOTROPIC_8X},
			{"RAY_TEXTURE_FILTER_ANISOTROPIC_16X", ray::TextureFilter::TEXTURE_FILTER_ANISOTROPIC_16X}
		});
		lua.new_enum<ray::TextureWrap>("rayTextureWrap", {
			{"RAY_TEXTURE_WRAP_REPEAT", ray::TextureWrap::TEXTURE_WRAP_REPEAT},
			{"RAY_TEXTURE_WRAP_CLAMP", ray::TextureWrap::TEXTURE_WRAP_CLAMP},
			{"RAY_TEXTURE_WRAP_MIRROR_REPEAT", ray::TextureWrap::TEXTURE_WRAP_MIRROR_REPEAT},
			{"RAY_TEXTURE_WRAP_MIRROR_CLAMP", ray::TextureWrap::TEXTURE_WRAP_MIRROR_CLAMP}
		});
		lua.new_enum<ray::CubemapLayout>("rayCubemapLayout", {
			{"RAY_CUBEMAP_LAYOUT_AUTO_DETECT", ray::CubemapLayout::CUBEMAP_LAYOUT_AUTO_DETECT},
			{"RAY_CUBEMAP_LAYOUT_LINE_VERTICAL", ray::CubemapLayout::CUBEMAP_LAYOUT_LINE_VERTICAL},
			{"RAY_CUBEMAP_LAYOUT_LINE_HORIZONTAL", ray::CubemapLayout::CUBEMAP_LAYOUT_LINE_HORIZONTAL},
			{"RAY_CUBEMAP_LAYOUT_CROSS_THREE_BY_FOUR", ray::CubemapLayout::CUBEMAP_LAYOUT_CROSS_THREE_BY_FOUR},
			{"RAY_CUBEMAP_LAYOUT_CROSS_FOUR_BY_THREE", ray::CubemapLayout::CUBEMAP_LAYOUT_CROSS_FOUR_BY_THREE}
		});
		lua.new_enum<ray::FontType>("rayFontType", {
			{"RAY_FONT_DEFAULT", ray::FontType::FONT_DEFAULT},
			{"RAY_FONT_BITMAP", ray::FontType::FONT_BITMAP},
			{"RAY_FONT_SDF", ray::FontType::FONT_SDF}
		});
		lua.new_enum<ray::BlendMode>("rayBlendMode", {
			{"RAY_BLEND_ALPHA", ray::BlendMode::BLEND_ALPHA},
			{"RAY_BLEND_ADDITIVE", ray::BlendMode::BLEND_ADDITIVE},
			{"RAY_BLEND_MULTIPLIED", ray::BlendMode::BLEND_MULTIPLIED},
			{"RAY_BLEND_ADD_COLORS", ray::BlendMode::BLEND_ADD_COLORS},
			{"RAY_BLEND_SUBTRACT_COLORS", ray::BlendMode::BLEND_SUBTRACT_COLORS},
			{"RAY_BLEND_ALPHA_PREMULTIPLY", ray::BlendMode::BLEND_ALPHA_PREMULTIPLY},
			{"RAY_BLEND_CUSTOM", ray::BlendMode::BLEND_CUSTOM},
			{"RAY_BLEND_CUSTOM_SEPARATE", ray::BlendMode::BLEND_CUSTOM_SEPARATE}
		});
		lua.new_enum<ray::Gesture>("rayGesture", {
			{"RAY_GESTURE_NONE", ray::Gesture::GESTURE_NONE},
			{"RAY_GESTURE_TAP", ray::Gesture::GESTURE_TAP},
			{"RAY_GESTURE_DOUBLETAP", ray::Gesture::GESTURE_DOUBLETAP},
			{"RAY_GESTURE_HOLD", ray::Gesture::GESTURE_HOLD},
			{"RAY_GESTURE_DRAG", ray::Gesture::GESTURE_DRAG},
			{"RAY_GESTURE_SWIPE_RIGHT", ray::Gesture::GESTURE_SWIPE_RIGHT},
			{"RAY_GESTURE_SWIPE_LEFT", ray::Gesture::GESTURE_SWIPE_LEFT},
			{"RAY_GESTURE_SWIPE_UP", ray::Gesture::GESTURE_SWIPE_UP},
			{"RAY_GESTURE_SWIPE_DOWN", ray::Gesture::GESTURE_SWIPE_DOWN},
			{"RAY_GESTURE_PINCH_IN", ray::Gesture::GESTURE_PINCH_IN},
			{"RAY_GESTURE_PINCH_OUT", ray::Gesture::GESTURE_PINCH_OUT}
		});
		lua.new_enum<ray::CameraMode>("rayCameraMode", {
			{"RAY_CAMERA_CUSTOM", ray::CameraMode::CAMERA_CUSTOM},
			{"RAY_CAMERA_FREE", ray::CameraMode::CAMERA_FREE},
			{"RAY_CAMERA_ORBITAL", ray::CameraMode::CAMERA_ORBITAL},
			{"RAY_CAMERA_FIRST_PERSON", ray::CameraMode::CAMERA_FIRST_PERSON},
			{"RAY_CAMERA_THIRD_PERSON", ray::CameraMode::CAMERA_THIRD_PERSON}
		});
		lua.new_enum<ray::CameraProjection>("rayCameraProjection", {
			{"RAY_CAMERA_PERSPECTIVE", ray::CameraProjection::CAMERA_PERSPECTIVE},
			{"RAY_CAMERA_ORTHOGRAPHIC", ray::CameraProjection::CAMERA_ORTHOGRAPHIC}
		});
		lua.new_enum<ray::NPatchLayout>("rayNPatchLayout", {
			{"RAY_NPATCH_NINE_PATCH", ray::NPatchLayout::NPATCH_NINE_PATCH},
			{"RAY_NPATCH_THREE_PATCH_VERTICAL", ray::NPatchLayout::NPATCH_THREE_PATCH_VERTICAL},
			{"RAY_NPATCH_THREE_PATCH_HORIZONTAL", ray::NPatchLayout::NPATCH_THREE_PATCH_HORIZONTAL}
		});
		// clang-format on

		lua.set_function("rayWindowShouldClose", &ray::WindowShouldClose);
		lua.set_function("rayIsWindowReady", &ray::IsWindowReady);
		lua.set_function("rayIsWindowFullscreen", &ray::IsWindowFullscreen);
		lua.set_function("rayIsWindowHidden", &ray::IsWindowHidden);
		lua.set_function("rayIsWindowMinimized", &ray::IsWindowMinimized);
		lua.set_function("rayIsWindowMaximized", &ray::IsWindowMaximized);
		lua.set_function("rayIsWindowFocused", &ray::IsWindowFocused);
		lua.set_function("rayIsWindowResized", &ray::IsWindowResized);
		lua.set_function("rayIsWindowState", &ray::IsWindowState);
		lua.set_function("raySetWindowState", &ray::SetWindowState);
		lua.set_function("rayClearWindowState", &ray::ClearWindowState);
		lua.set_function("rayToggleFullscreen", &ray::ToggleFullscreen);
		lua.set_function("rayToggleBorderlessWindowed", &ray::ToggleBorderlessWindowed);
		lua.set_function("rayMaximizeWindow", &ray::MaximizeWindow);
		lua.set_function("rayMinimizeWindow", &ray::MinimizeWindow);
		lua.set_function("rayRestoreWindow", &ray::RestoreWindow);
		lua.set_function("raySetWindowIcon", &ray::SetWindowIcon);
		lua.set_function("raySetWindowIcons", &ray::SetWindowIcons);
		lua.set_function("raySetWindowTitle", &ray::SetWindowTitle);
		lua.set_function("raySetWindowPosition", &ray::SetWindowPosition);
		lua.set_function("raySetWindowMonitor", &ray::SetWindowMonitor);
		lua.set_function("raySetWindowMinSize", &ray::SetWindowMinSize);
		lua.set_function("raySetWindowMaxSize", &ray::SetWindowMaxSize);
		lua.set_function("raySetWindowSize", &ray::SetWindowSize);
		lua.set_function("raySetWindowOpacity", &ray::SetWindowOpacity);
		lua.set_function("raySetWindowFocused", &ray::SetWindowFocused);
		lua.set_function("rayGetWindowHandle", &ray::GetWindowHandle);
		lua.set_function("rayGetScreenWidth", &ray::GetScreenWidth);
		lua.set_function("rayGetScreenHeight", &ray::GetScreenHeight);
		lua.set_function("rayGetRenderWidth", &ray::GetRenderWidth);
		lua.set_function("rayGetRenderHeight", &ray::GetRenderHeight);
		lua.set_function("rayGetMonitorCount", &ray::GetMonitorCount);
		lua.set_function("rayGetCurrentMonitor", &ray::GetCurrentMonitor);
		lua.set_function("rayGetMonitorPosition", &ray::GetMonitorPosition);
		lua.set_function("rayGetMonitorWidth", &ray::GetMonitorWidth);
		lua.set_function("rayGetMonitorHeight", &ray::GetMonitorHeight);
		lua.set_function("rayGetMonitorPhysicalWidth", &ray::GetMonitorPhysicalWidth);
		lua.set_function("rayGetMonitorPhysicalHeight", &ray::GetMonitorPhysicalHeight);
		lua.set_function("rayGetMonitorRefreshRate", &ray::GetMonitorRefreshRate);
		lua.set_function("rayGetWindowPosition", &ray::GetWindowPosition);
		lua.set_function("rayGetWindowScaleDPI", &ray::GetWindowScaleDPI);
		lua.set_function("rayGetMonitorName", &ray::GetMonitorName);
		lua.set_function("raySetClipboardText", &ray::SetClipboardText);
		lua.set_function("rayGetClipboardText", &ray::GetClipboardText);
		lua.set_function("rayGetClipboardImage", &ray::GetClipboardImage);
		lua.set_function("rayEnableEventWaiting", &ray::EnableEventWaiting);
		lua.set_function("rayDisableEventWaiting", &ray::DisableEventWaiting);
		lua.set_function("rayShowCursor", &ray::ShowCursor);
		lua.set_function("rayHideCursor", &ray::HideCursor);
		lua.set_function("rayIsCursorHidden", &ray::IsCursorHidden);
		lua.set_function("rayEnableCursor", &ray::EnableCursor);
		lua.set_function("rayDisableCursor", &ray::DisableCursor);
		lua.set_function("rayIsCursorOnScreen", &ray::IsCursorOnScreen);
		lua.set_function("rayClearBackground", &ray::ClearBackground);
		lua.set_function("rayBeginDrawing", &ray::BeginDrawing);
		lua.set_function("rayEndDrawing", &ray::EndDrawing);
		lua.set_function("rayBeginMode2D", &ray::BeginMode2D);
		lua.set_function("rayEndMode2D", &ray::EndMode2D);
		lua.set_function("rayBeginMode3D", &ray::BeginMode3D);
		lua.set_function("rayEndMode3D", &ray::EndMode3D);
		lua.set_function("rayBeginTextureMode", &ray::BeginTextureMode);
		lua.set_function("rayEndTextureMode", &ray::EndTextureMode);
		lua.set_function("rayBeginShaderMode", &ray::BeginShaderMode);
		lua.set_function("rayEndShaderMode", &ray::EndShaderMode);
		lua.set_function("rayBeginBlendMode", &ray::BeginBlendMode);
		lua.set_function("rayEndBlendMode", &ray::EndBlendMode);
		lua.set_function("rayBeginScissorMode", &ray::BeginScissorMode);
		lua.set_function("rayEndScissorMode", &ray::EndScissorMode);
		lua.set_function("rayLoadShader", &ray::LoadShader);
		lua.set_function("rayLoadShaderFromMemory", &ray::LoadShaderFromMemory);
		lua.set_function("rayIsShaderValid", &ray::IsShaderValid);
		lua.set_function("rayGetShaderLocation", &ray::GetShaderLocation);
		lua.set_function("rayGetShaderLocationAttrib", &ray::GetShaderLocationAttrib);
		lua.set_function("raySetShaderValue", &ray::SetShaderValue);
		lua.set_function("raySetShaderValueV", &ray::SetShaderValueV);
		lua.set_function("raySetShaderValueMatrix", &ray::SetShaderValueMatrix);
		lua.set_function("raySetShaderValueTexture", &ray::SetShaderValueTexture);
		lua.set_function("rayUnloadShader", &ray::UnloadShader);
		lua.set_function("rayGetScreenToWorldRay", &ray::GetScreenToWorldRay);
		lua.set_function("rayGetScreenToWorldRayEx", &ray::GetScreenToWorldRayEx);
		lua.set_function("rayGetWorldToScreen", &ray::GetWorldToScreen);
		lua.set_function("rayGetWorldToScreenEx", &ray::GetWorldToScreenEx);
		lua.set_function("rayGetWorldToScreen2D", &ray::GetWorldToScreen2D);
		lua.set_function("rayGetScreenToWorld2D", &ray::GetScreenToWorld2D);
		lua.set_function("rayGetCameraMatrix", &ray::GetCameraMatrix);
		lua.set_function("rayGetCameraMatrix2D", &ray::GetCameraMatrix2D);
		lua.set_function("raySetTargetFPS", &ray::SetTargetFPS);
		lua.set_function("rayGetFrameTime", &ray::GetFrameTime);
		lua.set_function("rayGetTime", &ray::GetTime);
		lua.set_function("rayGetFPS", &ray::GetFPS);
		lua.set_function("raySwapScreenBuffer", &ray::SwapScreenBuffer);
		lua.set_function("rayPollInputEvents", &ray::PollInputEvents);
		lua.set_function("rayWaitTime", &ray::WaitTime);
		lua.set_function("raySetRandomSeed", &ray::SetRandomSeed);
		lua.set_function("rayGetRandomValue", &ray::GetRandomValue);
		lua.set_function("rayLoadRandomSequence", &ray::LoadRandomSequence);
		lua.set_function("rayUnloadRandomSequence", &ray::UnloadRandomSequence);
		lua.set_function("rayTakeScreenshot", &ray::TakeScreenshot);
		lua.set_function("raySetConfigFlags", &ray::SetConfigFlags);
		lua.set_function("rayOpenURL", &ray::OpenURL);
		lua.set_function("rayLoadFileData", &ray::LoadFileData);
		lua.set_function("rayUnloadFileData", &ray::UnloadFileData);
		lua.set_function("raySaveFileData", &ray::SaveFileData);
		lua.set_function("rayExportDataAsCode", &ray::ExportDataAsCode);
		lua.set_function("rayLoadFileText", &ray::LoadFileText);
		lua.set_function("rayUnloadFileText", &ray::UnloadFileText);
		lua.set_function("raySaveFileText", &ray::SaveFileText);
		lua.set_function("raySetLoadFileDataCallback", &ray::SetLoadFileDataCallback);
		lua.set_function("raySetSaveFileDataCallback", &ray::SetSaveFileDataCallback);
		lua.set_function("raySetLoadFileTextCallback", &ray::SetLoadFileTextCallback);
		lua.set_function("raySetSaveFileTextCallback", &ray::SetSaveFileTextCallback);
		lua.set_function("rayFileRename", &ray::FileRename);
		lua.set_function("rayFileRemove", &ray::FileRemove);
		lua.set_function("rayFileCopy", &ray::FileCopy);
		lua.set_function("rayFileMove", &ray::FileMove);
		lua.set_function("rayFileTextReplace", &ray::FileTextReplace);
		lua.set_function("rayFileTextFindIndex", &ray::FileTextFindIndex);
		lua.set_function("rayFileExists", &ray::FileExists);
		lua.set_function("rayDirectoryExists", &ray::DirectoryExists);
		lua.set_function("rayIsFileExtension", &ray::IsFileExtension);
		lua.set_function("rayGetFileLength", &ray::GetFileLength);
		lua.set_function("rayGetFileModTime", &ray::GetFileModTime);
		lua.set_function("rayGetFileExtension", &ray::GetFileExtension);
		lua.set_function("rayGetFileName", &ray::GetFileName);
		lua.set_function("rayGetFileNameWithoutExt", &ray::GetFileNameWithoutExt);
		lua.set_function("rayGetDirectoryPath", &ray::GetDirectoryPath);
		lua.set_function("rayGetPrevDirectoryPath", &ray::GetPrevDirectoryPath);
		lua.set_function("rayGetWorkingDirectory", &ray::GetWorkingDirectory);
		lua.set_function("rayGetApplicationDirectory", &ray::GetApplicationDirectory);
		lua.set_function("rayMakeDirectory", &ray::MakeDirectory);
		lua.set_function("rayChangeDirectory", &ray::ChangeDirectory);
		lua.set_function("rayIsPathFile", &ray::IsPathFile);
		lua.set_function("rayIsFileNameValid", &ray::IsFileNameValid);
		lua.set_function("rayLoadDirectoryFiles", &ray::LoadDirectoryFiles);
		lua.set_function("rayLoadDirectoryFilesEx", &ray::LoadDirectoryFilesEx);
		lua.set_function("rayUnloadDirectoryFiles", &ray::UnloadDirectoryFiles);
		lua.set_function("rayIsFileDropped", &ray::IsFileDropped);
		lua.set_function("rayLoadDroppedFiles", &ray::LoadDroppedFiles);
		lua.set_function("rayUnloadDroppedFiles", &ray::UnloadDroppedFiles);
		lua.set_function("rayGetDirectoryFileCount", &ray::GetDirectoryFileCount);
		lua.set_function("rayGetDirectoryFileCountEx", &ray::GetDirectoryFileCountEx);
		lua.set_function("rayCompressData", &ray::CompressData);
		lua.set_function("rayDecompressData", &ray::DecompressData);
		lua.set_function("rayEncodeDataBase64", &ray::EncodeDataBase64);
		lua.set_function("rayDecodeDataBase64", &ray::DecodeDataBase64);
		lua.set_function("rayComputeCRC32", &ray::ComputeCRC32);
		lua.set_function("rayComputeMD5", &ray::ComputeMD5);
		lua.set_function("rayComputeSHA1", &ray::ComputeSHA1);
		lua.set_function("rayComputeSHA256", &ray::ComputeSHA256);
		lua.set_function("rayLoadAutomationEventList", &ray::LoadAutomationEventList);
		lua.set_function("rayUnloadAutomationEventList", &ray::UnloadAutomationEventList);
		lua.set_function("rayExportAutomationEventList", &ray::ExportAutomationEventList);
		lua.set_function("raySetAutomationEventList", &ray::SetAutomationEventList);
		lua.set_function("raySetAutomationEventBaseFrame", &ray::SetAutomationEventBaseFrame);
		lua.set_function("rayStartAutomationEventRecording", &ray::StartAutomationEventRecording);
		lua.set_function("rayStopAutomationEventRecording", &ray::StopAutomationEventRecording);
		lua.set_function("rayPlayAutomationEvent", &ray::PlayAutomationEvent);
		lua.set_function("rayIsKeyPressed", &ray::IsKeyPressed);
		lua.set_function("rayIsKeyPressedRepeat", &ray::IsKeyPressedRepeat);
		lua.set_function("rayIsKeyDown", &ray::IsKeyDown);
		lua.set_function("rayIsKeyReleased", &ray::IsKeyReleased);
		lua.set_function("rayIsKeyUp", &ray::IsKeyUp);
		lua.set_function("rayGetKeyPressed", &ray::GetKeyPressed);
		lua.set_function("rayGetCharPressed", &ray::GetCharPressed);
		lua.set_function("rayGetKeyName", &ray::GetKeyName);
		lua.set_function("raySetExitKey", &ray::SetExitKey);
		lua.set_function("rayIsGamepadAvailable", &ray::IsGamepadAvailable);
		lua.set_function("rayGetGamepadName", &ray::GetGamepadName);
		lua.set_function("rayIsGamepadButtonPressed", &ray::IsGamepadButtonPressed);
		lua.set_function("rayIsGamepadButtonDown", &ray::IsGamepadButtonDown);
		lua.set_function("rayIsGamepadButtonReleased", &ray::IsGamepadButtonReleased);
		lua.set_function("rayIsGamepadButtonUp", &ray::IsGamepadButtonUp);
		lua.set_function("rayGetGamepadButtonPressed", &ray::GetGamepadButtonPressed);
		lua.set_function("rayGetGamepadAxisCount", &ray::GetGamepadAxisCount);
		lua.set_function("rayGetGamepadAxisMovement", &ray::GetGamepadAxisMovement);
		lua.set_function("raySetGamepadMappings", &ray::SetGamepadMappings);
		lua.set_function("raySetGamepadVibration", &ray::SetGamepadVibration);
		lua.set_function("rayIsMouseButtonPressed", &ray::IsMouseButtonPressed);
		lua.set_function("rayIsMouseButtonDown", &ray::IsMouseButtonDown);
		lua.set_function("rayIsMouseButtonReleased", &ray::IsMouseButtonReleased);
		lua.set_function("rayIsMouseButtonUp", &ray::IsMouseButtonUp);
		lua.set_function("rayGetMouseX", &ray::GetMouseX);
		lua.set_function("rayGetMouseY", &ray::GetMouseY);
		lua.set_function("rayGetMousePosition", &ray::GetMousePosition);
		lua.set_function("rayGetMouseDelta", &ray::GetMouseDelta);
		lua.set_function("raySetMousePosition", &ray::SetMousePosition);
		lua.set_function("raySetMouseOffset", &ray::SetMouseOffset);
		lua.set_function("raySetMouseScale", &ray::SetMouseScale);
		lua.set_function("rayGetMouseWheelMove", &ray::GetMouseWheelMove);
		lua.set_function("rayGetMouseWheelMoveV", &ray::GetMouseWheelMoveV);
		lua.set_function("raySetMouseCursor", &ray::SetMouseCursor);
		lua.set_function("rayGetTouchX", &ray::GetTouchX);
		lua.set_function("rayGetTouchY", &ray::GetTouchY);
		lua.set_function("rayGetTouchPosition", &ray::GetTouchPosition);
		lua.set_function("rayGetTouchPointId", &ray::GetTouchPointId);
		lua.set_function("rayGetTouchPointCount", &ray::GetTouchPointCount);
		lua.set_function("raySetGesturesEnabled", &ray::SetGesturesEnabled);
		lua.set_function("rayIsGestureDetected", &ray::IsGestureDetected);
		lua.set_function("rayGetGestureDetected", &ray::GetGestureDetected);
		lua.set_function("rayGetGestureHoldDuration", &ray::GetGestureHoldDuration);
		lua.set_function("rayGetGestureDragVector", &ray::GetGestureDragVector);
		lua.set_function("rayGetGestureDragAngle", &ray::GetGestureDragAngle);
		lua.set_function("rayGetGesturePinchVector", &ray::GetGesturePinchVector);
		lua.set_function("rayGetGesturePinchAngle", &ray::GetGesturePinchAngle);
		lua.set_function("rayUpdateCamera", &ray::UpdateCamera);
		lua.set_function("rayUpdateCameraPro", &ray::UpdateCameraPro);
		lua.set_function("raySetShapesTexture", &ray::SetShapesTexture);
		lua.set_function("rayGetShapesTexture", &ray::GetShapesTexture);
		lua.set_function("rayGetShapesTextureRectangle", &ray::GetShapesTextureRectangle);
		lua.set_function("rayDrawPixel", &ray::DrawPixel);
		lua.set_function("rayDrawPixelV", &ray::DrawPixelV);
		lua.set_function("rayDrawLine", &ray::DrawLine);
		lua.set_function("rayDrawLineV", &ray::DrawLineV);
		lua.set_function("rayDrawLineEx", &ray::DrawLineEx);
		lua.set_function("rayDrawLineStrip", &ray::DrawLineStrip);
		lua.set_function("rayDrawLineBezier", &ray::DrawLineBezier);
		lua.set_function("rayDrawLineDashed", &ray::DrawLineDashed);
		lua.set_function("rayDrawCircle", &ray::DrawCircle);
		lua.set_function("rayDrawCircleV", &ray::DrawCircleV);
		lua.set_function("rayDrawCircleGradient", &ray::DrawCircleGradient);
		lua.set_function("rayDrawCircleSector", &ray::DrawCircleSector);
		lua.set_function("rayDrawCircleSectorLines", &ray::DrawCircleSectorLines);
		lua.set_function("rayDrawCircleLines", &ray::DrawCircleLines);
		lua.set_function("rayDrawCircleLinesV", &ray::DrawCircleLinesV);
		lua.set_function("rayDrawEllipse", &ray::DrawEllipse);
		lua.set_function("rayDrawEllipseV", &ray::DrawEllipseV);
		lua.set_function("rayDrawEllipseLines", &ray::DrawEllipseLines);
		lua.set_function("rayDrawEllipseLinesV", &ray::DrawEllipseLinesV);
		lua.set_function("rayDrawRing", &ray::DrawRing);
		lua.set_function("rayDrawRingLines", &ray::DrawRingLines);
		lua.set_function("rayDrawRectangle", &ray::DrawRectangle);
		lua.set_function("rayDrawRectangleV", &ray::DrawRectangleV);
		lua.set_function("rayDrawRectangleRec", &ray::DrawRectangleRec);
		lua.set_function("rayDrawRectanglePro", &ray::DrawRectanglePro);
		lua.set_function("rayDrawRectangleGradientV", &ray::DrawRectangleGradientV);
		lua.set_function("rayDrawRectangleGradientH", &ray::DrawRectangleGradientH);
		lua.set_function("rayDrawRectangleGradientEx", &ray::DrawRectangleGradientEx);
		lua.set_function("rayDrawRectangleLines", &ray::DrawRectangleLines);
		lua.set_function("rayDrawRectangleLinesEx", &ray::DrawRectangleLinesEx);
		lua.set_function("rayDrawRectangleRounded", &ray::DrawRectangleRounded);
		lua.set_function("rayDrawRectangleRoundedLines", &ray::DrawRectangleRoundedLines);
		lua.set_function("rayDrawRectangleRoundedLinesEx", &ray::DrawRectangleRoundedLinesEx);
		lua.set_function("rayDrawTriangle", &ray::DrawTriangle);
		lua.set_function("rayDrawTriangleGradient", &ray::DrawTriangleGradient);
		lua.set_function("rayDrawTriangleLines", &ray::DrawTriangleLines);
		lua.set_function("rayDrawTriangleFan", &ray::DrawTriangleFan);
		lua.set_function("rayDrawTriangleStrip", &ray::DrawTriangleStrip);
		lua.set_function("rayDrawPoly", &ray::DrawPoly);
		lua.set_function("rayDrawPolyLines", &ray::DrawPolyLines);
		lua.set_function("rayDrawPolyLinesEx", &ray::DrawPolyLinesEx);
		lua.set_function("rayDrawSplineLinear", &ray::DrawSplineLinear);
		lua.set_function("rayDrawSplineBasis", &ray::DrawSplineBasis);
		lua.set_function("rayDrawSplineCatmullRom", &ray::DrawSplineCatmullRom);
		lua.set_function("rayDrawSplineBezierQuadratic", &ray::DrawSplineBezierQuadratic);
		lua.set_function("rayDrawSplineBezierCubic", &ray::DrawSplineBezierCubic);
		lua.set_function("rayDrawSplineSegmentLinear", &ray::DrawSplineSegmentLinear);
		lua.set_function("rayDrawSplineSegmentBasis", &ray::DrawSplineSegmentBasis);
		lua.set_function("rayDrawSplineSegmentCatmullRom", &ray::DrawSplineSegmentCatmullRom);
		lua.set_function("rayDrawSplineSegmentBezierQuadratic", &ray::DrawSplineSegmentBezierQuadratic);
		lua.set_function("rayDrawSplineSegmentBezierCubic", &ray::DrawSplineSegmentBezierCubic);
		lua.set_function("rayGetSplinePointLinear", &ray::GetSplinePointLinear);
		lua.set_function("rayGetSplinePointBasis", &ray::GetSplinePointBasis);
		lua.set_function("rayGetSplinePointCatmullRom", &ray::GetSplinePointCatmullRom);
		lua.set_function("rayGetSplinePointBezierQuadratic", &ray::GetSplinePointBezierQuadratic);
		lua.set_function("rayGetSplinePointBezierCubic", &ray::GetSplinePointBezierCubic);
		lua.set_function("rayCheckCollisionRecs", &ray::CheckCollisionRecs);
		lua.set_function("rayCheckCollisionCircles", &ray::CheckCollisionCircles);
		lua.set_function("rayCheckCollisionCircleRec", &ray::CheckCollisionCircleRec);
		lua.set_function("rayCheckCollisionCircleLine", &ray::CheckCollisionCircleLine);
		lua.set_function("rayCheckCollisionPointRec", &ray::CheckCollisionPointRec);
		lua.set_function("rayCheckCollisionPointCircle", &ray::CheckCollisionPointCircle);
		lua.set_function("rayCheckCollisionPointTriangle", &ray::CheckCollisionPointTriangle);
		lua.set_function("rayCheckCollisionPointLine", &ray::CheckCollisionPointLine);
		lua.set_function("rayCheckCollisionPointPoly", &ray::CheckCollisionPointPoly);
		lua.set_function("rayCheckCollisionLines", &ray::CheckCollisionLines);
		lua.set_function("rayGetCollisionRec", &ray::GetCollisionRec);
		lua.set_function("rayLoadImage", &ray::LoadImage);
		lua.set_function("rayLoadImageRaw", &ray::LoadImageRaw);
		lua.set_function("rayLoadImageAnim", &ray::LoadImageAnim);
		lua.set_function("rayLoadImageAnimFromMemory", &ray::LoadImageAnimFromMemory);
		lua.set_function("rayLoadImageFromMemory", &ray::LoadImageFromMemory);
		lua.set_function("rayLoadImageFromTexture", &ray::LoadImageFromTexture);
		lua.set_function("rayLoadImageFromScreen", &ray::LoadImageFromScreen);
		lua.set_function("rayIsImageValid", &ray::IsImageValid);
		lua.set_function("rayUnloadImage", &ray::UnloadImage);
		lua.set_function("rayExportImage", &ray::ExportImage);
		lua.set_function("rayExportImageToMemory", &ray::ExportImageToMemory);
		lua.set_function("rayExportImageAsCode", &ray::ExportImageAsCode);
		lua.set_function("rayGenImageColor", &ray::GenImageColor);
		lua.set_function("rayGenImageGradientLinear", &ray::GenImageGradientLinear);
		lua.set_function("rayGenImageGradientRadial", &ray::GenImageGradientRadial);
		lua.set_function("rayGenImageGradientSquare", &ray::GenImageGradientSquare);
		lua.set_function("rayGenImageChecked", &ray::GenImageChecked);
		lua.set_function("rayGenImageWhiteNoise", &ray::GenImageWhiteNoise);
		lua.set_function("rayGenImagePerlinNoise", &ray::GenImagePerlinNoise);
		lua.set_function("rayGenImageCellular", &ray::GenImageCellular);
		lua.set_function("rayGenImageText", &ray::GenImageText);
		lua.set_function("rayImageCopy", &ray::ImageCopy);
		lua.set_function("rayImageFromImage", &ray::ImageFromImage);
		lua.set_function("rayImageFromChannel", &ray::ImageFromChannel);
		lua.set_function("rayImageText", &ray::ImageText);
		lua.set_function("rayImageTextEx", &ray::ImageTextEx);
		lua.set_function("rayImageFormat", &ray::ImageFormat);
		lua.set_function("rayImageToPOT", &ray::ImageToPOT);
		lua.set_function("rayImageCrop", &ray::ImageCrop);
		lua.set_function("rayImageAlphaCrop", &ray::ImageAlphaCrop);
		lua.set_function("rayImageAlphaClear", &ray::ImageAlphaClear);
		lua.set_function("rayImageAlphaMask", &ray::ImageAlphaMask);
		lua.set_function("rayImageAlphaPremultiply", &ray::ImageAlphaPremultiply);
		lua.set_function("rayImageBlurGaussian", &ray::ImageBlurGaussian);
		lua.set_function("rayImageKernelConvolution", &ray::ImageKernelConvolution);
		lua.set_function("rayImageResize", &ray::ImageResize);
		lua.set_function("rayImageResizeNN", &ray::ImageResizeNN);
		lua.set_function("rayImageResizeCanvas", &ray::ImageResizeCanvas);
		lua.set_function("rayImageMipmaps", &ray::ImageMipmaps);
		lua.set_function("rayImageDither", &ray::ImageDither);
		lua.set_function("rayImageFlipVertical", &ray::ImageFlipVertical);
		lua.set_function("rayImageFlipHorizontal", &ray::ImageFlipHorizontal);
		lua.set_function("rayImageRotate", &ray::ImageRotate);
		lua.set_function("rayImageRotateCW", &ray::ImageRotateCW);
		lua.set_function("rayImageRotateCCW", &ray::ImageRotateCCW);
		lua.set_function("rayImageColorTint", &ray::ImageColorTint);
		lua.set_function("rayImageColorInvert", &ray::ImageColorInvert);
		lua.set_function("rayImageColorGrayscale", &ray::ImageColorGrayscale);
		lua.set_function("rayImageColorContrast", &ray::ImageColorContrast);
		lua.set_function("rayImageColorBrightness", &ray::ImageColorBrightness);
		lua.set_function("rayImageColorReplace", &ray::ImageColorReplace);
		lua.set_function("rayLoadImageColors", &ray::LoadImageColors);
		lua.set_function("rayLoadImagePalette", &ray::LoadImagePalette);
		lua.set_function("rayUnloadImageColors", &ray::UnloadImageColors);
		lua.set_function("rayUnloadImagePalette", &ray::UnloadImagePalette);
		lua.set_function("rayGetImageAlphaBorder", &ray::GetImageAlphaBorder);
		lua.set_function("rayGetImageColor", &ray::GetImageColor);
		lua.set_function("rayLoadTexture", &ray::LoadTexture);
		lua.set_function("rayLoadTextureFromImage", &ray::LoadTextureFromImage);
		lua.set_function("rayLoadTextureCubemap", &ray::LoadTextureCubemap);
		lua.set_function("rayLoadRenderTexture", &ray::LoadRenderTexture);
		lua.set_function("rayIsTextureValid", &ray::IsTextureValid);
		lua.set_function("rayUnloadTexture", &ray::UnloadTexture);
		lua.set_function("rayIsRenderTextureValid", &ray::IsRenderTextureValid);
		lua.set_function("rayUnloadRenderTexture", &ray::UnloadRenderTexture);
		lua.set_function("rayUpdateTexture", &ray::UpdateTexture);
		lua.set_function("rayUpdateTextureRec", &ray::UpdateTextureRec);
		lua.set_function("rayGenTextureMipmaps", &ray::GenTextureMipmaps);
		lua.set_function("raySetTextureFilter", &ray::SetTextureFilter);
		lua.set_function("raySetTextureWrap", &ray::SetTextureWrap);
		lua.set_function("rayDrawTexture", &ray::DrawTexture);
		lua.set_function("rayDrawTextureV", &ray::DrawTextureV);
		lua.set_function("rayDrawTextureEx", &ray::DrawTextureEx);
		lua.set_function("rayDrawTextureRec", &ray::DrawTextureRec);
		lua.set_function("rayDrawTexturePro", &ray::DrawTexturePro);
		lua.set_function("rayDrawTextureNPatch", &ray::DrawTextureNPatch);
		lua.set_function("rayColorIsEqual", &ray::ColorIsEqual);
		lua.set_function("rayFade", &ray::Fade);
		lua.set_function("rayColorToInt", &ray::ColorToInt);
		lua.set_function("rayColorNormalize", &ray::ColorNormalize);
		lua.set_function("rayColorFromNormalized", &ray::ColorFromNormalized);
		lua.set_function("rayColorToHSV", &ray::ColorToHSV);
		lua.set_function("rayColorFromHSV", &ray::ColorFromHSV);
		lua.set_function("rayColorTint", &ray::ColorTint);
		lua.set_function("rayColorBrightness", &ray::ColorBrightness);
		lua.set_function("rayColorContrast", &ray::ColorContrast);
		lua.set_function("rayColorAlpha", &ray::ColorAlpha);
		lua.set_function("rayColorAlphaBlend", &ray::ColorAlphaBlend);
		lua.set_function("rayColorLerp", &ray::ColorLerp);
		lua.set_function("rayGetColor", &ray::GetColor);
		lua.set_function("rayGetPixelColor", &ray::GetPixelColor);
		lua.set_function("raySetPixelColor", &ray::SetPixelColor);
		lua.set_function("rayGetPixelDataSize", &ray::GetPixelDataSize);
		lua.set_function("rayGetFontDefault", &ray::GetFontDefault);
		lua.set_function("rayLoadFont", &ray::LoadFont);
		lua.set_function("rayLoadFontEx", &ray::LoadFontEx);
		lua.set_function("rayLoadFontFromImage", &ray::LoadFontFromImage);
		lua.set_function("rayLoadFontFromMemory", &ray::LoadFontFromMemory);
		lua.set_function("rayIsFontValid", &ray::IsFontValid);
		lua.set_function("rayLoadFontData", &ray::LoadFontData);
		lua.set_function("rayGenImageFontAtlas", &ray::GenImageFontAtlas);
		lua.set_function("rayUnloadFontData", &ray::UnloadFontData);
		lua.set_function("rayUnloadFont", &ray::UnloadFont);
		lua.set_function("rayExportFontAsCode", &ray::ExportFontAsCode);
		lua.set_function("rayDrawFPS", &ray::DrawFPS);
		lua.set_function("rayDrawText", &ray::DrawText);
		lua.set_function("rayDrawTextEx", &ray::DrawTextEx);
		lua.set_function("rayDrawTextPro", &ray::DrawTextPro);
		lua.set_function("rayDrawTextCodepoint", &ray::DrawTextCodepoint);
		lua.set_function("rayDrawTextCodepoints", &ray::DrawTextCodepoints);
		lua.set_function("raySetTextLineSpacing", &ray::SetTextLineSpacing);
		lua.set_function("rayMeasureText", &ray::MeasureText);
		lua.set_function("rayMeasureTextEx", &ray::MeasureTextEx);
		lua.set_function("rayMeasureTextCodepoints", &ray::MeasureTextCodepoints);
		lua.set_function("rayGetGlyphIndex", &ray::GetGlyphIndex);
		lua.set_function("rayGetGlyphInfo", &ray::GetGlyphInfo);
		lua.set_function("rayGetGlyphAtlasRec", &ray::GetGlyphAtlasRec);
		lua.set_function("rayLoadUTF8", &ray::LoadUTF8);
		lua.set_function("rayUnloadUTF8", &ray::UnloadUTF8);
		lua.set_function("rayLoadCodepoints", &ray::LoadCodepoints);
		lua.set_function("rayUnloadCodepoints", &ray::UnloadCodepoints);
		lua.set_function("rayGetCodepointCount", &ray::GetCodepointCount);
		lua.set_function("rayGetCodepoint", &ray::GetCodepoint);
		lua.set_function("rayGetCodepointNext", &ray::GetCodepointNext);
		lua.set_function("rayGetCodepointPrevious", &ray::GetCodepointPrevious);
		lua.set_function("rayCodepointToUTF8", &ray::CodepointToUTF8);
		lua.set_function("rayLoadTextLines", &ray::LoadTextLines);
		lua.set_function("rayUnloadTextLines", &ray::UnloadTextLines);
		lua.set_function("rayTextCopy", &ray::TextCopy);
		lua.set_function("rayTextIsEqual", &ray::TextIsEqual);
		lua.set_function("rayTextLength", &ray::TextLength);
		lua.set_function("rayTextFormat", &ray::TextFormat);
		lua.set_function("rayTextSubtext", &ray::TextSubtext);
		lua.set_function("rayTextRemoveSpaces", &ray::TextRemoveSpaces);
		lua.set_function("rayGetTextBetween", &ray::GetTextBetween);
		lua.set_function("rayTextReplace", &ray::TextReplace);
		lua.set_function("rayTextReplaceAlloc", &ray::TextReplaceAlloc);
		lua.set_function("rayTextReplaceBetween", &ray::TextReplaceBetween);
		lua.set_function("rayTextReplaceBetweenAlloc", &ray::TextReplaceBetweenAlloc);
		lua.set_function("rayTextInsert", &ray::TextInsert);
		lua.set_function("rayTextInsertAlloc", &ray::TextInsertAlloc);
		lua.set_function("rayTextJoin", &ray::TextJoin);
		lua.set_function("rayTextSplit", &ray::TextSplit);
		lua.set_function("rayTextAppend", &ray::TextAppend);
		lua.set_function("rayTextFindIndex", &ray::TextFindIndex);
		lua.set_function("rayTextToUpper", &ray::TextToUpper);
		lua.set_function("rayTextToLower", &ray::TextToLower);
		lua.set_function("rayTextToPascal", &ray::TextToPascal);
		lua.set_function("rayTextToSnake", &ray::TextToSnake);
		lua.set_function("rayTextToCamel", &ray::TextToCamel);
		lua.set_function("rayTextToInteger", &ray::TextToInteger);
		lua.set_function("rayTextToFloat", &ray::TextToFloat);
		lua.set_function("rayDrawLine3D", &ray::DrawLine3D);
		lua.set_function("rayDrawPoint3D", &ray::DrawPoint3D);
		lua.set_function("rayDrawCircle3D", &ray::DrawCircle3D);
		lua.set_function("rayDrawTriangle3D", &ray::DrawTriangle3D);
		lua.set_function("rayDrawTriangleStrip3D", &ray::DrawTriangleStrip3D);
		lua.set_function("rayDrawCube", &ray::DrawCube);
		lua.set_function("rayDrawCubeV", &ray::DrawCubeV);
		lua.set_function("rayDrawCubeWires", &ray::DrawCubeWires);
		lua.set_function("rayDrawCubeWiresV", &ray::DrawCubeWiresV);
		lua.set_function("rayDrawSphere", &ray::DrawSphere);
		lua.set_function("rayDrawSphereEx", &ray::DrawSphereEx);
		lua.set_function("rayDrawSphereWires", &ray::DrawSphereWires);
		lua.set_function("rayDrawCylinder", &ray::DrawCylinder);
		lua.set_function("rayDrawCylinderEx", &ray::DrawCylinderEx);
		lua.set_function("rayDrawCylinderWires", &ray::DrawCylinderWires);
		lua.set_function("rayDrawCylinderWiresEx", &ray::DrawCylinderWiresEx);
		lua.set_function("rayDrawCapsule", &ray::DrawCapsule);
		lua.set_function("rayDrawCapsuleWires", &ray::DrawCapsuleWires);
		lua.set_function("rayDrawPlane", &ray::DrawPlane);
		lua.set_function("rayDrawRay", &ray::DrawRay);
		lua.set_function("rayDrawGrid", &ray::DrawGrid);
		lua.set_function("rayLoadModel", &ray::LoadModel);
		lua.set_function("rayLoadModelFromMesh", &ray::LoadModelFromMesh);
		lua.set_function("rayIsModelValid", &ray::IsModelValid);
		lua.set_function("rayUnloadModel", &ray::UnloadModel);
		lua.set_function("rayGetModelBoundingBox", &ray::GetModelBoundingBox);
		lua.set_function("rayDrawModel", &ray::DrawModel);
		lua.set_function("rayDrawModelEx", &ray::DrawModelEx);
		lua.set_function("rayDrawModelWires", &ray::DrawModelWires);
		lua.set_function("rayDrawModelWiresEx", &ray::DrawModelWiresEx);
		lua.set_function("rayDrawBoundingBox", &ray::DrawBoundingBox);
		lua.set_function("rayDrawBillboard", &ray::DrawBillboard);
		lua.set_function("rayDrawBillboardRec", &ray::DrawBillboardRec);
		lua.set_function("rayDrawBillboardPro", &ray::DrawBillboardPro);
		lua.set_function("rayUploadMesh", &ray::UploadMesh);
		lua.set_function("rayUpdateMeshBuffer", &ray::UpdateMeshBuffer);
		lua.set_function("rayUnloadMesh", &ray::UnloadMesh);
		lua.set_function("rayDrawMesh", &ray::DrawMesh);
		lua.set_function("rayDrawMeshInstanced", &ray::DrawMeshInstanced);
		lua.set_function("rayGetMeshBoundingBox", &ray::GetMeshBoundingBox);
		lua.set_function("rayGenMeshTangents", &ray::GenMeshTangents);
		lua.set_function("rayExportMesh", &ray::ExportMesh);
		lua.set_function("rayExportMeshAsCode", &ray::ExportMeshAsCode);
		lua.set_function("rayGenMeshPoly", &ray::GenMeshPoly);
		lua.set_function("rayGenMeshPlane", &ray::GenMeshPlane);
		lua.set_function("rayGenMeshCube", &ray::GenMeshCube);
		lua.set_function("rayGenMeshSphere", &ray::GenMeshSphere);
		lua.set_function("rayGenMeshHemiSphere", &ray::GenMeshHemiSphere);
		lua.set_function("rayGenMeshCylinder", &ray::GenMeshCylinder);
		lua.set_function("rayGenMeshCone", &ray::GenMeshCone);
		lua.set_function("rayGenMeshTorus", &ray::GenMeshTorus);
		lua.set_function("rayGenMeshKnot", &ray::GenMeshKnot);
		lua.set_function("rayGenMeshHeightmap", &ray::GenMeshHeightmap);
		lua.set_function("rayGenMeshCubicmap", &ray::GenMeshCubicmap);
		lua.set_function("rayLoadMaterials", &ray::LoadMaterials);
		lua.set_function("rayLoadMaterialDefault", &ray::LoadMaterialDefault);
		lua.set_function("rayIsMaterialValid", &ray::IsMaterialValid);
		lua.set_function("rayUnloadMaterial", &ray::UnloadMaterial);
		lua.set_function("raySetMaterialTexture", &ray::SetMaterialTexture);
		lua.set_function("raySetModelMeshMaterial", &ray::SetModelMeshMaterial);
		lua.set_function("rayLoadModelAnimations", &ray::LoadModelAnimations);
		lua.set_function("rayUpdateModelAnimation", &ray::UpdateModelAnimation);
		lua.set_function("rayUpdateModelAnimationEx", &ray::UpdateModelAnimationEx);
		lua.set_function("rayUnloadModelAnimations", &ray::UnloadModelAnimations);
		lua.set_function("rayIsModelAnimationValid", &ray::IsModelAnimationValid);
		lua.set_function("rayCheckCollisionSpheres", &ray::CheckCollisionSpheres);
		lua.set_function("rayCheckCollisionBoxes", &ray::CheckCollisionBoxes);
		lua.set_function("rayCheckCollisionBoxSphere", &ray::CheckCollisionBoxSphere);
		lua.set_function("rayGetRayCollisionSphere", &ray::GetRayCollisionSphere);
		lua.set_function("rayGetRayCollisionBox", &ray::GetRayCollisionBox);
		lua.set_function("rayGetRayCollisionMesh", &ray::GetRayCollisionMesh);
		lua.set_function("rayGetRayCollisionTriangle", &ray::GetRayCollisionTriangle);
		lua.set_function("rayGetRayCollisionQuad", &ray::GetRayCollisionQuad);
		lua.set_function("rayInitAudioDevice", &ray::InitAudioDevice);
		lua.set_function("rayCloseAudioDevice", &ray::CloseAudioDevice);
		lua.set_function("rayIsAudioDeviceReady", &ray::IsAudioDeviceReady);
		lua.set_function("raySetMasterVolume", &ray::SetMasterVolume);
		lua.set_function("rayGetMasterVolume", &ray::GetMasterVolume);
		lua.set_function("rayLoadWave", &ray::LoadWave);
		lua.set_function("rayLoadWaveFromMemory", &ray::LoadWaveFromMemory);
		lua.set_function("rayIsWaveValid", &ray::IsWaveValid);
		lua.set_function("rayLoadSound", &ray::LoadSound);
		lua.set_function("rayLoadSoundFromWave", &ray::LoadSoundFromWave);
		lua.set_function("rayLoadSoundAlias", &ray::LoadSoundAlias);
		lua.set_function("rayIsSoundValid", &ray::IsSoundValid);
		lua.set_function("rayUpdateSound", &ray::UpdateSound);
		lua.set_function("rayUnloadWave", &ray::UnloadWave);
		lua.set_function("rayUnloadSound", &ray::UnloadSound);
		lua.set_function("rayUnloadSoundAlias", &ray::UnloadSoundAlias);
		lua.set_function("rayExportWave", &ray::ExportWave);
		lua.set_function("rayExportWaveAsCode", &ray::ExportWaveAsCode);
		lua.set_function("rayPlaySound", &ray::PlaySound);
		lua.set_function("rayStopSound", &ray::StopSound);
		lua.set_function("rayPauseSound", &ray::PauseSound);
		lua.set_function("rayResumeSound", &ray::ResumeSound);
		lua.set_function("rayIsSoundPlaying", &ray::IsSoundPlaying);
		lua.set_function("raySetSoundVolume", &ray::SetSoundVolume);
		lua.set_function("raySetSoundPitch", &ray::SetSoundPitch);
		lua.set_function("raySetSoundPan", &ray::SetSoundPan);
		lua.set_function("rayWaveCopy", &ray::WaveCopy);
		lua.set_function("rayWaveCrop", &ray::WaveCrop);
		lua.set_function("rayWaveFormat", &ray::WaveFormat);
		lua.set_function("rayLoadWaveSamples", &ray::LoadWaveSamples);
		lua.set_function("rayUnloadWaveSamples", &ray::UnloadWaveSamples);
		lua.set_function("rayLoadMusicStream", &ray::LoadMusicStream);
		lua.set_function("rayLoadMusicStreamFromMemory", &ray::LoadMusicStreamFromMemory);
		lua.set_function("rayIsMusicValid", &ray::IsMusicValid);
		lua.set_function("rayUnloadMusicStream", &ray::UnloadMusicStream);
		lua.set_function("rayPlayMusicStream", &ray::PlayMusicStream);
		lua.set_function("rayIsMusicStreamPlaying", &ray::IsMusicStreamPlaying);
		lua.set_function("rayUpdateMusicStream", &ray::UpdateMusicStream);
		lua.set_function("rayStopMusicStream", &ray::StopMusicStream);
		lua.set_function("rayPauseMusicStream", &ray::PauseMusicStream);
		lua.set_function("rayResumeMusicStream", &ray::ResumeMusicStream);
		lua.set_function("raySeekMusicStream", &ray::SeekMusicStream);
		lua.set_function("raySetMusicVolume", &ray::SetMusicVolume);
		lua.set_function("raySetMusicPitch", &ray::SetMusicPitch);
		lua.set_function("raySetMusicPan", &ray::SetMusicPan);
		lua.set_function("rayGetMusicTimeLength", &ray::GetMusicTimeLength);
		lua.set_function("rayGetMusicTimePlayed", &ray::GetMusicTimePlayed);
		lua.set_function("rayLoadAudioStream", &ray::LoadAudioStream);
		lua.set_function("rayIsAudioStreamValid", &ray::IsAudioStreamValid);
		lua.set_function("rayUnloadAudioStream", &ray::UnloadAudioStream);
		lua.set_function("rayUpdateAudioStream", &ray::UpdateAudioStream);
		lua.set_function("rayIsAudioStreamProcessed", &ray::IsAudioStreamProcessed);
		lua.set_function("rayPlayAudioStream", &ray::PlayAudioStream);
		lua.set_function("rayPauseAudioStream", &ray::PauseAudioStream);
		lua.set_function("rayResumeAudioStream", &ray::ResumeAudioStream);
		lua.set_function("rayIsAudioStreamPlaying", &ray::IsAudioStreamPlaying);
		lua.set_function("rayStopAudioStream", &ray::StopAudioStream);
		lua.set_function("raySetAudioStreamVolume", &ray::SetAudioStreamVolume);
		lua.set_function("raySetAudioStreamPitch", &ray::SetAudioStreamPitch);
		lua.set_function("raySetAudioStreamPan", &ray::SetAudioStreamPan);
		lua.set_function("raySetAudioStreamBufferSizeDefault", &ray::SetAudioStreamBufferSizeDefault);
		lua.set_function("raySetAudioStreamCallback", &ray::SetAudioStreamCallback);
		lua.set_function("rayAttachAudioStreamProcessor", &ray::AttachAudioStreamProcessor);
		lua.set_function("rayDetachAudioStreamProcessor", &ray::DetachAudioStreamProcessor);
		lua.set_function("rayAttachAudioMixedProcessor", &ray::AttachAudioMixedProcessor);
		lua.set_function("rayClamp", &ray::Clamp);
		lua.set_function("rayLerp", &ray::Lerp);
		lua.set_function("rayNormalize", &ray::Normalize);
		lua.set_function("rayRemap", &ray::Remap);
		lua.set_function("rayWrap", &ray::Wrap);
		lua.set_function("rayFloatEquals", &ray::FloatEquals);
		lua.set_function("rayVector2Zero", &ray::Vector2Zero);
		lua.set_function("rayVector2One", &ray::Vector2One);
		lua.set_function("rayVector2Add", &ray::Vector2Add);
		lua.set_function("rayVector2AddValue", &ray::Vector2AddValue);
		lua.set_function("rayVector2Subtract", &ray::Vector2Subtract);
		lua.set_function("rayVector2SubtractValue", &ray::Vector2SubtractValue);
		lua.set_function("rayVector2Length", &ray::Vector2Length);
		lua.set_function("rayVector2LengthSqr", &ray::Vector2LengthSqr);
		lua.set_function("rayVector2DotProduct", &ray::Vector2DotProduct);
		lua.set_function("rayVector2CrossProduct", &ray::Vector2CrossProduct);
		lua.set_function("rayVector2Distance", &ray::Vector2Distance);
		lua.set_function("rayVector2DistanceSqr", &ray::Vector2DistanceSqr);
		lua.set_function("rayVector2Angle", &ray::Vector2Angle);
		lua.set_function("rayVector2LineAngle", &ray::Vector2LineAngle);
		lua.set_function("rayVector2Scale", &ray::Vector2Scale);
		lua.set_function("rayVector2Multiply", &ray::Vector2Multiply);
		lua.set_function("rayVector2Negate", &ray::Vector2Negate);
		lua.set_function("rayVector2Divide", &ray::Vector2Divide);
		lua.set_function("rayVector2Normalize", &ray::Vector2Normalize);
		lua.set_function("rayVector2Transform", &ray::Vector2Transform);
		lua.set_function("rayVector2Lerp", &ray::Vector2Lerp);
		lua.set_function("rayVector2Reflect", &ray::Vector2Reflect);
		lua.set_function("rayVector2Min", &ray::Vector2Min);
		lua.set_function("rayVector2Max", &ray::Vector2Max);
		lua.set_function("rayVector2Rotate", &ray::Vector2Rotate);
		lua.set_function("rayVector2MoveTowards", &ray::Vector2MoveTowards);
		lua.set_function("rayVector2Invert", &ray::Vector2Invert);
		lua.set_function("rayVector2Clamp", &ray::Vector2Clamp);
		lua.set_function("rayVector2ClampValue", &ray::Vector2ClampValue);
		lua.set_function("rayVector2Equals", &ray::Vector2Equals);
		lua.set_function("rayVector2Refract", &ray::Vector2Refract);
		lua.set_function("rayVector3Zero", &ray::Vector3Zero);
		lua.set_function("rayVector3One", &ray::Vector3One);
		lua.set_function("rayVector3Add", &ray::Vector3Add);
		lua.set_function("rayVector3AddValue", &ray::Vector3AddValue);
		lua.set_function("rayVector3Subtract", &ray::Vector3Subtract);
		lua.set_function("rayVector3SubtractValue", &ray::Vector3SubtractValue);
		lua.set_function("rayVector3Scale", &ray::Vector3Scale);
		lua.set_function("rayVector3Multiply", &ray::Vector3Multiply);
		lua.set_function("rayVector3CrossProduct", &ray::Vector3CrossProduct);
		lua.set_function("rayVector3Perpendicular", &ray::Vector3Perpendicular);
		lua.set_function("rayVector3Length", &ray::Vector3Length);
		lua.set_function("rayVector3LengthSqr", &ray::Vector3LengthSqr);
		lua.set_function("rayVector3DotProduct", &ray::Vector3DotProduct);
		lua.set_function("rayVector3Distance", &ray::Vector3Distance);
		lua.set_function("rayVector3DistanceSqr", &ray::Vector3DistanceSqr);
		lua.set_function("rayVector3Angle", &ray::Vector3Angle);
		lua.set_function("rayVector3Negate", &ray::Vector3Negate);
		lua.set_function("rayVector3Divide", &ray::Vector3Divide);
		lua.set_function("rayVector3Normalize", &ray::Vector3Normalize);
		lua.set_function("rayVector3Project", &ray::Vector3Project);
		lua.set_function("rayVector3Reject", &ray::Vector3Reject);
		lua.set_function("rayVector3OrthoNormalize", &ray::Vector3OrthoNormalize);
		lua.set_function("rayVector3Transform", &ray::Vector3Transform);
		lua.set_function("rayVector3RotateByQuaternion", &ray::Vector3RotateByQuaternion);
		lua.set_function("rayVector3RotateByAxisAngle", &ray::Vector3RotateByAxisAngle);
		lua.set_function("rayVector3MoveTowards", &ray::Vector3MoveTowards);
		lua.set_function("rayVector3Lerp", &ray::Vector3Lerp);
		lua.set_function("rayVector3CubicHermite", &ray::Vector3CubicHermite);
		lua.set_function("rayVector3Reflect", &ray::Vector3Reflect);
		lua.set_function("rayVector3Min", &ray::Vector3Min);
		lua.set_function("rayVector3Max", &ray::Vector3Max);
		lua.set_function("rayVector3Barycenter", &ray::Vector3Barycenter);
		lua.set_function("rayVector3Unproject", &ray::Vector3Unproject);
		lua.set_function("rayVector3ToFloatV", &ray::Vector3ToFloatV);
		lua.set_function("rayVector3Invert", &ray::Vector3Invert);
		lua.set_function("rayVector3Clamp", &ray::Vector3Clamp);
		lua.set_function("rayVector3ClampValue", &ray::Vector3ClampValue);
		lua.set_function("rayVector3Equals", &ray::Vector3Equals);
		lua.set_function("rayVector3Refract", &ray::Vector3Refract);
		lua.set_function("rayVector4Zero", &ray::Vector4Zero);
		lua.set_function("rayVector4One", &ray::Vector4One);
		lua.set_function("rayVector4Add", &ray::Vector4Add);
		lua.set_function("rayVector4AddValue", &ray::Vector4AddValue);
		lua.set_function("rayVector4Subtract", &ray::Vector4Subtract);
		lua.set_function("rayVector4SubtractValue", &ray::Vector4SubtractValue);
		lua.set_function("rayVector4Length", &ray::Vector4Length);
		lua.set_function("rayVector4LengthSqr", &ray::Vector4LengthSqr);
		lua.set_function("rayVector4DotProduct", &ray::Vector4DotProduct);
		lua.set_function("rayVector4Distance", &ray::Vector4Distance);
		lua.set_function("rayVector4DistanceSqr", &ray::Vector4DistanceSqr);
		lua.set_function("rayVector4Scale", &ray::Vector4Scale);
		lua.set_function("rayVector4Multiply", &ray::Vector4Multiply);
		lua.set_function("rayVector4Negate", &ray::Vector4Negate);
		lua.set_function("rayVector4Divide", &ray::Vector4Divide);
		lua.set_function("rayVector4Normalize", &ray::Vector4Normalize);
		lua.set_function("rayVector4Min", &ray::Vector4Min);
		lua.set_function("rayVector4Max", &ray::Vector4Max);
		lua.set_function("rayVector4Lerp", &ray::Vector4Lerp);
		lua.set_function("rayVector4MoveTowards", &ray::Vector4MoveTowards);
		lua.set_function("rayVector4Invert", &ray::Vector4Invert);
		lua.set_function("rayVector4Equals", &ray::Vector4Equals);
		lua.set_function("rayMatrixDeterminant", &ray::MatrixDeterminant);
		lua.set_function("rayMatrixTrace", &ray::MatrixTrace);
		lua.set_function("rayMatrixTranspose", &ray::MatrixTranspose);
		lua.set_function("rayMatrixInvert", &ray::MatrixInvert);
		lua.set_function("rayMatrixIdentity", &ray::MatrixIdentity);
		lua.set_function("rayMatrixAdd", &ray::MatrixAdd);
		lua.set_function("rayMatrixSubtract", &ray::MatrixSubtract);
		lua.set_function("rayMatrixMultiply", &ray::MatrixMultiply);
		lua.set_function("rayMatrixMultiplyValue", &ray::MatrixMultiplyValue);
		lua.set_function("rayMatrixTranslate", &ray::MatrixTranslate);
		lua.set_function("rayMatrixRotate", &ray::MatrixRotate);
		lua.set_function("rayMatrixRotateX", &ray::MatrixRotateX);
		lua.set_function("rayMatrixRotateY", &ray::MatrixRotateY);
		lua.set_function("rayMatrixRotateZ", &ray::MatrixRotateZ);
		lua.set_function("rayMatrixRotateXYZ", &ray::MatrixRotateXYZ);
		lua.set_function("rayMatrixRotateZYX", &ray::MatrixRotateZYX);
		lua.set_function("rayMatrixScale", &ray::MatrixScale);
		lua.set_function("rayMatrixFrustum", &ray::MatrixFrustum);
		lua.set_function("rayMatrixPerspective", &ray::MatrixPerspective);
		lua.set_function("rayMatrixOrtho", &ray::MatrixOrtho);
		lua.set_function("rayMatrixLookAt", &ray::MatrixLookAt);
		lua.set_function("rayMatrixToFloatV", &ray::MatrixToFloatV);
		lua.set_function("rayQuaternionAdd", &ray::QuaternionAdd);
		lua.set_function("rayQuaternionAddValue", &ray::QuaternionAddValue);
		lua.set_function("rayQuaternionSubtract", &ray::QuaternionSubtract);
		lua.set_function("rayQuaternionSubtractValue", &ray::QuaternionSubtractValue);
		lua.set_function("rayQuaternionIdentity", &ray::QuaternionIdentity);
		lua.set_function("rayQuaternionLength", &ray::QuaternionLength);
		lua.set_function("rayQuaternionNormalize", &ray::QuaternionNormalize);
		lua.set_function("rayQuaternionInvert", &ray::QuaternionInvert);
		lua.set_function("rayQuaternionMultiply", &ray::QuaternionMultiply);
		lua.set_function("rayQuaternionScale", &ray::QuaternionScale);
		lua.set_function("rayQuaternionDivide", &ray::QuaternionDivide);
		lua.set_function("rayQuaternionLerp", &ray::QuaternionLerp);
		lua.set_function("rayQuaternionNlerp", &ray::QuaternionNlerp);
		lua.set_function("rayQuaternionSlerp", &ray::QuaternionSlerp);
		lua.set_function("rayQuaternionCubicHermiteSpline", &ray::QuaternionCubicHermiteSpline);
		lua.set_function("rayQuaternionFromVector3ToVector3", &ray::QuaternionFromVector3ToVector3);
		lua.set_function("rayQuaternionFromMatrix", &ray::QuaternionFromMatrix);
		lua.set_function("rayQuaternionToMatrix", &ray::QuaternionToMatrix);
		lua.set_function("rayQuaternionFromAxisAngle", &ray::QuaternionFromAxisAngle);
		lua.set_function("rayQuaternionToAxisAngle", &ray::QuaternionToAxisAngle);
		lua.set_function("rayQuaternionFromEuler", &ray::QuaternionFromEuler);
		lua.set_function("rayQuaternionToEuler", &ray::QuaternionToEuler);
		lua.set_function("rayQuaternionTransform", &ray::QuaternionTransform);
		lua.set_function("rayQuaternionEquals", &ray::QuaternionEquals);
		lua.set_function("rayMatrixCompose", &ray::MatrixCompose);
		lua.set_function("rayMatrixDecompose", &ray::MatrixDecompose);
		lua.set_function("rayGetCameraForward", &ray::GetCameraForward);
		lua.set_function("rayGetCameraUp", &ray::GetCameraUp);
		lua.set_function("rayGetCameraRight", &ray::GetCameraRight);
		lua.set_function("rayCameraMoveForward", &ray::CameraMoveForward);
		lua.set_function("rayCameraMoveUp", &ray::CameraMoveUp);
		lua.set_function("rayCameraMoveRight", &ray::CameraMoveRight);
		lua.set_function("rayCameraMoveToTarget", &ray::CameraMoveToTarget);
		lua.set_function("rayCameraYaw", &ray::CameraYaw);
		lua.set_function("rayCameraPitch", &ray::CameraPitch);
		lua.set_function("rayCameraRoll", &ray::CameraRoll);
		lua.set_function("rayGetCameraViewMatrix", &ray::GetCameraViewMatrix);
		lua.set_function("rayGetCameraProjectionMatrix", &ray::GetCameraProjectionMatrix);

		// lua.set_function("rayBeginVrStereoMode", &ray::BeginVrStereoMode);
		// lua.set_function("rayEndVrStereoMode", &ray::EndVrStereoMode);
		// lua.set_function("rayLoadVrStereoConfig", &ray::LoadVrStereoConfig);
		// lua.set_function("rayUnloadVrStereoConfig", &ray::UnloadVrStereoConfig);
		// lua.set_function("rayInitWindow", &ray::InitWindow);
		// lua.set_function("rayCloseWindow", &ray::CloseWindow);
		// lua.set_function("raySetTraceLogLevel", &ray::SetTraceLogLevel);
		// lua.set_function("rayTraceLog", &ray::TraceLog);
		// lua.set_function("raySetTraceLogCallback", &ray::SetTraceLogCallback);
		// lua.set_function("rayMemAlloc", &ray::MemAlloc);
		// lua.set_function("rayMemRealloc", &ray::MemRealloc);
		// lua.set_function("rayMemFree", &ray::MemFree);
		// lua.set_function("ImageClearBackground", &ray::ImageClearBackground);
		// lua.set_function("ImageDrawPixel", &ray::ImageDrawPixel);
		// lua.set_function("ImageDrawPixelV", &ray::ImageDrawPixelV);
		// lua.set_function("ImageDrawLine", &ray::ImageDrawLine);
		// lua.set_function("ImageDrawLineV", &ray::ImageDrawLineV);
		// lua.set_function("ImageDrawLineEx", &ray::ImageDrawLineEx);
		// lua.set_function("ImageDrawCircle", &ray::ImageDrawCircle);
		// lua.set_function("ImageDrawCircleV", &ray::ImageDrawCircleV);
		// lua.set_function("ImageDrawCircleLines", &ray::ImageDrawCircleLines);
		// lua.set_function("ImageDrawCircleLinesV", &ray::ImageDrawCircleLinesV);
		// lua.set_function("ImageDrawRectangle", &ray::ImageDrawRectangle);
		// lua.set_function("ImageDrawRectangleV", &ray::ImageDrawRectangleV);
		// lua.set_function("ImageDrawRectangleRec", &ray::ImageDrawRectangleRec);
		// lua.set_function("ImageDrawRectangleLines", &ray::ImageDrawRectangleLines);
		// lua.set_function("ImageDrawRectangleLinesEx", &ray::ImageDrawRectangleLinesEx);
		// lua.set_function("ImageDrawTriangle", &ray::ImageDrawTriangle);
		// lua.set_function("ImageDrawTriangleGradient", &ray::ImageDrawTriangleGradient);
		// lua.set_function("ImageDrawTriangleLines", &ray::ImageDrawTriangleLines);
		// lua.set_function("ImageDrawTriangleFan", &ray::ImageDrawTriangleFan);
		// lua.set_function("ImageDrawTriangleStrip", &ray::ImageDrawTriangleStrip);
		// lua.set_function("ImageDraw", &ray::ImageDraw);
		// lua.set_function("ImageDrawText", &ray::ImageDrawText);
		// lua.set_function("ImageDrawTextEx", &ray::ImageDrawTextEx);

		lua["RAY_LIGHTGRAY"]  = ray::Color {200, 200, 200, 255};
		lua["RAY_GRAY"]       = ray::Color {130, 130, 130, 255};
		lua["RAY_DARKGRAY"]   = ray::Color {80, 80, 80, 255};
		lua["RAY_YELLOW"]     = ray::Color {253, 249, 0, 255};
		lua["RAY_GOLD"]       = ray::Color {255, 203, 0, 255};
		lua["RAY_ORANGE"]     = ray::Color {255, 161, 0, 255};
		lua["RAY_PINK"]       = ray::Color {255, 109, 194, 255};
		lua["RAY_RED"]        = ray::Color {230, 41, 55, 255};
		lua["RAY_MAROON"]     = ray::Color {190, 33, 55, 255};
		lua["RAY_GREEN"]      = ray::Color {0, 228, 48, 255};
		lua["RAY_LIME"]       = ray::Color {0, 158, 47, 255};
		lua["RAY_DARKGREEN"]  = ray::Color {0, 117, 44, 255};
		lua["RAY_SKYBLUE"]    = ray::Color {102, 191, 255, 255};
		lua["RAY_BLUE"]       = ray::Color {0, 121, 241, 255};
		lua["RAY_DARKBLUE"]   = ray::Color {0, 82, 172, 255};
		lua["RAY_PURPLE"]     = ray::Color {200, 122, 255, 255};
		lua["RAY_VIOLET"]     = ray::Color {135, 60, 190, 255};
		lua["RAY_DARKPURPLE"] = ray::Color {112, 31, 126, 255};
		lua["RAY_BEIGE"]      = ray::Color {211, 176, 131, 255};
		lua["RAY_BROWN"]      = ray::Color {127, 106, 79, 255};
		lua["RAY_DARKBROWN"]  = ray::Color {76, 63, 47, 255};
		lua["RAY_WHITE"]      = ray::Color {255, 255, 255, 255};
		lua["RAY_BLACK"]      = ray::Color {0, 0, 0, 255};
		lua["RAY_BLANK"]      = ray::Color {0, 0, 0, 0};
		lua["RAY_MAGENTA"]    = ray::Color {255, 0, 255, 255};
		lua["RAY_RAYWHITE"]   = ray::Color {245, 245, 245, 255};

		lua.set("RAY_PI", RAY_PI);
		lua.set("RAY_EPSILON", RAY_EPSILON);
		lua.set("RAY_DEG2RAD", RAY_DEG2RAD);
		lua.set("RAY_RAD2DEG", RAY_RAD2DEG);
		// RAY_MatrixToFloat
		// RAY_Vector3ToFloat
	}
} // namespace galaxy
