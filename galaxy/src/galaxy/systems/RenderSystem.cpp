///
/// RenderSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/ParticleGenerator.hpp"
#include "galaxy/components/Primitive.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Text.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/flags/Disabled.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/scene/Scene.hpp"

#include "RenderSystem.hpp"

namespace galaxy
{
	namespace systems
	{
		RenderSystem::RenderSystem()
		{
		}

		RenderSystem::~RenderSystem()
		{
		}

		void RenderSystem::update(scene::Scene* scene)
		{
			// Possible to do this in parallel but overhead would cost more than perf saved.
			auto& renderer = core::ServiceLocator<graphics::Renderer>::ref();

			const auto spr_group  = scene->m_world.m_registry.group<components::Sprite>(entt::get<components::Transform>, entt::exclude<flags::Disabled>);
			const auto prim_group = scene->m_world.m_registry.group<components::Primitive>(entt::get<components::Transform>, entt::exclude<flags::Disabled>);
			const auto text_group = scene->m_world.m_registry.group<components::Text>(entt::get<components::Transform>, entt::exclude<flags::Disabled>);
			const auto pg_view    = scene->m_world.m_registry.view<components::ParticleGenerator>(entt::exclude<flags::Disabled>);

			graphics::RenderCommand cmd;

			for (auto&& [entity, sprite, transform] : spr_group.each())
			{
				transform.set_origin(sprite.get_width() * 0.5f, sprite.get_height() * 0.5f);

				cmd.instances              = sprite.get_instances();
				cmd.mode                   = sprite.get_mode();
				cmd.m_index_count          = sprite.get_count();
				cmd.m_layer                = sprite.get_layer();
				cmd.m_texture              = sprite.get_texture();
				cmd.m_vao                  = sprite.get_vao();
				cmd.uniform_data.entity    = static_cast<int>(entt::to_integral(entity));
				cmd.uniform_data.colour    = {1.0f, 1.0f, 1.0f, sprite.get_opacity()};
				cmd.uniform_data.transform = transform.get_transform();
				cmd.uniform_data.point     = false;
				cmd.uniform_data.textured  = true;
				cmd.m_particle             = false;

				renderer.submit(cmd);
			}

			for (auto&& [entity, primitive, transform] : prim_group.each())
			{
				transform.set_origin(primitive.get_width() * 0.5f, primitive.get_height() * 0.5f);

				cmd.instances              = primitive.get_instances();
				cmd.mode                   = primitive.get_mode();
				cmd.m_index_count          = primitive.get_count();
				cmd.m_layer                = primitive.get_layer();
				cmd.m_texture              = primitive.get_texture();
				cmd.m_vao                  = primitive.get_vao();
				cmd.uniform_data.entity    = static_cast<int>(entt::to_integral(entity));
				cmd.uniform_data.colour    = primitive.m_colour.to_vec4();
				cmd.uniform_data.transform = transform.get_transform();
				cmd.uniform_data.point     = primitive.get_shape() == math::Shape::POINT;
				cmd.uniform_data.textured  = false;
				cmd.m_particle             = false;

				renderer.submit(cmd);
			}

			for (auto&& [entity, text, transform] : text_group.each())
			{
				transform.set_origin(text.get_width() * 0.5f, text.get_height() * 0.5f);

				cmd.instances              = text.get_instances();
				cmd.mode                   = text.get_mode();
				cmd.m_index_count          = text.get_count();
				cmd.m_layer                = text.get_layer();
				cmd.m_texture              = text.get_texture();
				cmd.m_vao                  = text.get_vao();
				cmd.uniform_data.entity    = static_cast<int>(entt::to_integral(entity));
				cmd.uniform_data.colour    = text.m_colour.to_vec4();
				cmd.uniform_data.transform = transform.get_transform();
				cmd.uniform_data.point     = false;
				cmd.uniform_data.textured  = true;
				cmd.m_particle             = false;

				renderer.submit(cmd);
			}

			for (auto&& [entity, pg] : pg_view.each())
			{
				cmd.instances             = pg.get_instances();
				cmd.mode                  = pg.get_mode();
				cmd.m_index_count         = pg.get_count();
				cmd.m_layer               = pg.get_layer();
				cmd.m_texture             = pg.get_texture();
				cmd.m_vao                 = pg.get_vao();
				cmd.uniform_data.entity   = static_cast<int>(entt::to_integral(entity));
				cmd.uniform_data.point    = false;
				cmd.uniform_data.textured = true;
				cmd.m_particle            = true;

				pg.buffer_instances();
			}
		}
	} // namespace systems
} // namespace galaxy

/*
  #include "ParticleSystemLauncher.h"

  #include <cstdio>
  #include <glad/glad.h>
  #include <imgui/imgui.h>
  #include <imgui/imgui_impl_glfw.h>
  #include <imgui/imgui_impl_opengl3.h>

  #include "InputManager.h"

  #define GL_SILENCE_DEPRECATION
  #if defined(IMGUI_IMPL_OPENGL_ES2)
  #include <GLES2/gl2.h>
  #endif
  #include <glad/glad.h>
  #include <iostream>
  #include <GLFW/glfw3.h> // Will drag system OpenGL headers
  #include <cstdlib>

  #if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
  #pragma comment(lib, "legacy_stdio_definitions")
  #endif

  #include "Scene/Scene.h"

  #ifdef __EMSCRIPTEN__
  #include "imgui/libs/emscripten/emscripten_mainloop_stub.h"
  #include <emscripten/html5.h>
  #endif

  static void glfw_error_callback(int error, const char* description) {
	  fprintf(stderr, "GLFW Error %d: %s\n", error, description);
  }

  ParticleSystemLauncher::ParticleSystemLauncher() {
	  // Setup window
	  glfwSetErrorCallback(glfw_error_callback);
	  if (!glfwInit())
		  exit(1);

  // Decide GL+GLSL versions
  #if defined(IMGUI_IMPL_OPENGL_ES2)
	  const char* glsl_version = "#version 300 es";
	  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
  #elif defined(__APPLE__)
	  const char* glsl_version = "#version 330";
	  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
	  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
  #else
	  const char* glsl_version = "#version 330";
	  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
	  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // 3.0+ only
  #endif

  #ifdef __EMSCRIPTEN__
	  emscripten_get_canvas_element_size("#canvas", &display_w, &display_h);
  #endif

	  // Create window with graphics context
	  window = glfwCreateWindow(display_w, display_h, PROJECT_NAME.data(), nullptr, nullptr);
	  if (window == nullptr)
		  exit(1);
	  glfwMakeContextCurrent(window);
	  glfwSwapInterval(1); // Enable vsync

	  //    // Initialize GLFW callbacks
	  //    glfwSetWindowUserPointer(window, this);
	  glfwSetKeyCallback(window, InputManager::key_callback);
	  //    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	  //    glfwSetScrollCallback(window, InputManager::scroll_callback);
	  //    glfwSetCursorPosCallback(window, InputManager::cursor_position_callback);
	  //    glfwSetMouseButtonCallback(window, InputManager::mouse_button_callback);

	  // Center window
	  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	  const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	  auto xPos = (mode->width - display_w) / 2;
	  auto yPos = (mode->height - display_h) / 2;
	  glfwSetWindowPos(window, xPos, yPos);

  #ifdef __EMSCRIPTEN__
	  // Initialize OpenGL loader
	  if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress))
		  exit(1);
  #else
	  // Initialize OpenGL loader
	  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		  exit(1);
  #endif

	  // Setup Dear ImGui context
	  IMGUI_CHECKVERSION();
	  ImGui::CreateContext();
	  ImGuiIO& io = ImGui::GetIO();
	  (void)io;
	  //    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
	  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
	  // io.ConfigViewportsNoAutoMerge = true;
	  // io.ConfigViewportsNoTaskBarIcon = true;

	  // Setup Dear ImGui style
	  ImGui::StyleColorsDark();
	  // ImGui::StyleColorsLight();

	  // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	  ImGuiStyle& style = ImGui::GetStyle();
	  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	  {
		  style.WindowRounding = 0.0f;
		  style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	  }

	  // Setup Platform/Renderer backends
	  ImGui_ImplGlfw_InitForOpenGL(window, true);
	  ImGui_ImplOpenGL3_Init(glsl_version);

	  // Print OpenGL version
	  printf("OpenGL vendor: %s\nOpenGL version: %s\nGLSL version: %s\nGLFW version: %s\n"
			 "Glad version: %s\nImGui version: %s\nGLM version: %s\n",
		  getOpenGLVendor().data(),
		  getOpenGLVersion().data(),
		  getGLSLVersion().data(),
		  getGLFWVersion().data(),
		  getGladVersion().data(),
		  getImGuiVersion().data(),
		  getGLMVersion().data());

	  glEnable(GL_DEPTH_TEST);
	  glDepthFunc(GL_LESS);
	  glEnable(GL_BLEND);
	  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

  ParticleSystemLauncher::~ParticleSystemLauncher() {
	  // Cleanup
	  ImGui_ImplOpenGL3_Shutdown();
	  ImGui_ImplGlfw_Shutdown();
	  ImGui::DestroyContext();

	  glfwDestroyWindow(window);
	  glfwTerminate();
  }

  void ParticleSystemLauncher::start() {
	  // Create the scene
	  scene = std::make_unique<Scene>(display_w, display_h);

	  // Variables for the main loop
	  float deltaTime;
	  float accumulator = 0.0f;

	  // Main loop
  #ifdef __EMSCRIPTEN__
	  // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
	  // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
	  ImGuiIO& io = ImGui::GetIO();
	  (void)io;
	  io.IniFilename = NULL;
	  EMSCRIPTEN_MAINLOOP_BEGIN
  #else
	  while (!glfwWindowShouldClose(window))
  #endif
	  {
		  deltaTime = ImGui::GetIO().DeltaTime;

		  handleInputs();

		  handleUi(deltaTime);

		  updateGame(deltaTime);

		  accumulator += deltaTime;
		  while (accumulator >= fixedDeltaTime)
		  {
			  fixedUpdateGame(fixedDeltaTime);
			  accumulator -= fixedDeltaTime;
		  }

		  updateScreen();
	  }
  #ifdef __EMSCRIPTEN__
	  EMSCRIPTEN_MAINLOOP_END;
  #endif

	  scene.reset();
  }

  void ParticleSystemLauncher::handleInputs() {
	  glfwPollEvents();

if (InputManager::isLeftKeyPressed(window))
	scene->camera.moveLeft();

if (InputManager::isRightKeyPressed(window))
	scene->camera.moveRight();

if (InputManager::isForwardKeyPressed(window))
	scene->camera.moveForward();

if (InputManager::isBackwardKeyPressed(window))
	scene->camera.moveBackward();

if (InputManager::isUpKeyPressed(window))
	scene->camera.moveUp();

if (InputManager::isDownKeyPressed(window))
	scene->camera.moveDown();

if (InputManager::isPauseKeyPressed(window))
	scene->togglePause();

double x = 0, y = 0;
InputManager::getMouseMovement(window, x, y, InputManager::isKeyMouseMovementPressed(window));
scene->camera.processMouseMovement((float)x, (float)y);
}

void ParticleSystemLauncher::handleUi(float deltaTime)
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	{
#ifdef __EMSCRIPTEN__
		static bool isCollapsed = true;
		ImGui::SetNextWindowPos(ImVec2(-display_w / 2, -display_h / 2), ImGuiCond_Once);
		ImGui::SetNextWindowCollapsed(isCollapsed, ImGuiCond_Once);
#endif
		ImGui::Begin("Window info");
		ImGui::Text("%.3f ms/frame (%.1f FPS)", deltaTime, 1.0f / deltaTime);
		ImGui::Text("Window width: %d", display_w);
		ImGui::Text("Window height: %d", display_h);
		ImGui::Text("GPU: %s", getOpenGLVendor().data());
		ImGui::Text("OpenGL version: %s", getOpenGLVersion().data());
		ImGui::Text("GLSL version: %s", getGLSLVersion().data());
		ImGui::End();
	}

	{
#ifdef __EMSCRIPTEN__
		static bool isCollapsed = true;
		ImGui::SetNextWindowPos(ImVec2(-display_w / 2, (-display_h / 2) + 20), ImGuiCond_Once);
		ImGui::SetNextWindowCollapsed(isCollapsed, ImGuiCond_Once);
#endif
		ImGui::Begin("Camera settings");

#ifndef __EMSCRIPTEN__
		static bool wireframe = false;
		ImGui::TextColored(ImVec4(1.0F, 0.0F, 1.0F, 1.0F), "View settings");
		ImGui::Checkbox("Wireframe", &wireframe);
		if (wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		ImGui::NewLine();
#endif

		ImGui::TextColored(ImVec4(1.0F, 0.0F, 1.0F, 1.0F), "Camera settings");

		ImGui::Text("Position:");
		ImGui::DragFloat3("##position", (float*)&scene->camera.position);

		ImGui::NewLine();
		ImGui::Text("Pitch:");
		ImGui::Checkbox("Pitch constrained", &scene->camera.constrainPitch);
		ImGui::DragFloat("##pitch", &scene->camera.pitch);

		ImGui::Text("Yaw:");
		ImGui::DragFloat("##yaw", &scene->camera.yaw);

		ImGui::NewLine();
		ImGui::Text("FOV:");
		ImGui::DragFloat("##fov", &scene->camera.fov);

		ImGui::NewLine();
		ImGui::Text("Near plane:");
		ImGui::DragFloat("##near", &scene->camera.nearPlane);

		ImGui::Text("Far plane:");
		ImGui::DragFloat("##far", &scene->camera.farPlane);

		ImGui::NewLine();
		ImGui::Text("Speed:");
		ImGui::DragFloat("##speed", &scene->camera.movementSpeed);

		ImGui::Text("Sensitivity: ");
		ImGui::DragFloat("##sensitivity", &scene->camera.rotationSpeed, 0.1f);

		ImGui::End();
	}

	{
#ifdef __EMSCRIPTEN__
		static bool isCollapsed = true;
		ImGui::SetNextWindowPos(ImVec2(-display_w / 2, (-display_h / 2) + 40), ImGuiCond_Once);
		ImGui::SetNextWindowCollapsed(isCollapsed, ImGuiCond_Once);
#endif
		ImGui::Begin("Particle settings");

		ImGui::TextColored(ImVec4(1.0F, 0.0F, 1.0F, 1.0F), "Scene settings");
		ImGui::Text("Fixed time step: %f", fixedDeltaTime);

		ImGui::NewLine();
		ImGui::TextColored(ImVec4(1.0F, 0.0F, 1.0F, 1.0F), "Reset particles");
		ImGui::Button("Restart system");
		if (ImGui::IsItemClicked())
		{
			scene->particleGenerator.resetParticles();
		}

		ImGui::NewLine();
		ImGui::TextColored(ImVec4(1.0F, 0.0F, 1.0F, 1.0F), "Particles count");
		ImGui::Text("Current count: %d", scene->particleGenerator.getParticlesCount());

		static int newParticlesCount = 10000;
		ImGui::Text("New count:");
		ImGui::SameLine();
		ImGui::DragInt("##particlesCount", &newParticlesCount, 1, 1, 1000);
		ImGui::Button("Validate##validateParticlesCount");
		if (ImGui::IsItemClicked())
		{
			scene->particleGenerator.setParticlesCount(newParticlesCount);
		}

		ImGui::NewLine();
		ImGui::TextColored(ImVec4(1.0F, 0.0F, 1.0F, 1.0F), "Settings");
		ImGui::Text("Origin:");
		ImGui::DragFloat3("##origin", (float*)&scene->particleGenerator.position);

		ImGui::NewLine();
		ImGui::Text("Forces sum:");
		ImGui::DragFloat3("##forces", (float*)&scene->particleGenerator.sumForces);

		ImGui::NewLine();
		ImGui::Checkbox("Randomize initial velocity", &scene->particleGenerator.randomizeInitialVelocity);
		if (scene->particleGenerator.randomizeInitialVelocity)
		{
			ImGui::Text("Minimum initial velocity:");
			ImGui::DragFloat3("##minInitialVelocity", (float*)&scene->particleGenerator.minInitialVelocity);
			ImGui::Text("Maximum initial velocity:");
			ImGui::DragFloat3("##maxInitialVelocity", (float*)&scene->particleGenerator.maxInitialVelocity);
		}
		else
		{
			ImGui::Text("Initial velocity:");
			ImGui::DragFloat3("##initialVelocity", (float*)&scene->particleGenerator.fixedInitialVelocity);
		}

		ImGui::NewLine();
		ImGui::Checkbox("Randomize spread", &scene->particleGenerator.randomizePosition);
		if (scene->particleGenerator.randomizePosition)
		{
			ImGui::Text("Spread type:");
			ImGui::Selectable("- Sphere", scene->particleGenerator.spreadType == SpreadType::SPREAD_TYPE_SPHERE);
			if (ImGui::IsItemActivated())
			{
				scene->particleGenerator.spreadType = SpreadType::SPREAD_TYPE_SPHERE;
			}
			ImGui::Selectable("- Rectangle", scene->particleGenerator.spreadType == SpreadType::SPREAD_TYPE_RECTANGLE);
			if (ImGui::IsItemClicked())
			{
				scene->particleGenerator.spreadType = SpreadType::SPREAD_TYPE_RECTANGLE;
			}

			ImGui::NewLine();
			switch (scene->particleGenerator.spreadType)
			{
				case SpreadType::SPREAD_TYPE_SPHERE:
					ImGui::Text("Radius:");
					ImGui::DragFloat("##radius", &scene->particleGenerator.spreadRadius, 0.1f);
					break;
				case SpreadType::SPREAD_TYPE_RECTANGLE:
					ImGui::Text("Particles min spread:");
					ImGui::DragFloat3("##minSpread", (float*)&scene->particleGenerator.minRectangleSpread, 0.1f);
					ImGui::Text("Particles max spread:");
					ImGui::DragFloat3("##maxSpread", (float*)&scene->particleGenerator.maxRectangleSpread, 0.1f);
					break;
			}
		}

		ImGui::NewLine();
		ImGui::Checkbox("Randomize lifetime", &scene->particleGenerator.randomizeLifeTime);
		if (scene->particleGenerator.randomizeLifeTime)
		{
			ImGui::Text("Particle min lifetime:");
			ImGui::DragFloat("##minLifetime", &scene->particleGenerator.minLifeTime, 0.1f);
			ImGui::Text("Particle max lifetime:");
			ImGui::DragFloat("##maxLifetime", &scene->particleGenerator.maxLifeTime, 0.1f);
		}
		else
		{
			ImGui::Text("Particle lifetime:");
			ImGui::DragFloat("##lifetime", &scene->particleGenerator.fixedLifeTime, 0.1f);
		}

		ImGui::NewLine();
		ImGui::Checkbox("Randomize scale", &scene->particleGenerator.randomizeScale);
		if (scene->particleGenerator.randomizeScale)
		{
			ImGui::Checkbox("Keep aspect ratio", &scene->particleGenerator.keepScaleAspectRatio);
			if (scene->particleGenerator.keepScaleAspectRatio)
			{
				ImGui::Text("Particle min scale:");
				ImGui::DragFloat("##minScale", &scene->particleGenerator.minScale.x, 0.1f);
				ImGui::Text("Particle max scale:");
				ImGui::DragFloat("##maxScale", &scene->particleGenerator.maxScale.x, 0.1f);
			}
			else
			{
				ImGui::Text("Particle min scale:");
				ImGui::DragFloat2("##minScale", (float*)&scene->particleGenerator.minScale, 0.1f);
				ImGui::Text("Particle max scale:");
				ImGui::DragFloat2("##maxScale", (float*)&scene->particleGenerator.maxScale, 0.1f);
			}
		}
		else
		{
			ImGui::Text("Particle scale:");
			ImGui::DragFloat2("##scale", (float*)&scene->particleGenerator.fixedScale, 0.1f);
		}

		ImGui::NewLine();
		ImGui::Checkbox("Randomize color", &scene->particleGenerator.randomizeColor);
		if (scene->particleGenerator.randomizeColor)
		{
			ImGui::Text("Particle min color:");
			ImGui::ColorEdit3("##minColor", (float*)&scene->particleGenerator.minColor);
			ImGui::Text("Particle max color:");
			ImGui::ColorEdit3("##maxColor", (float*)&scene->particleGenerator.maxColor);
		}
		else
		{
			ImGui::Text("Particle color:");
			ImGui::ColorEdit3("##color", (float*)&scene->particleGenerator.fixedColor);
		}

		ImGui::NewLine();
		ImGui::Checkbox("Randomize transparency", &scene->particleGenerator.randomizeColorAlpha);
		if (scene->particleGenerator.randomizeColorAlpha)
		{
			ImGui::Text("Particle min transparency:");
			ImGui::DragFloat("##minAlpha", &scene->particleGenerator.minColorAlpha, 0.1f);
			ImGui::Text("Particle max transparency:");
			ImGui::DragFloat("##maxAlpha", &scene->particleGenerator.maxColorAlpha, 0.1f);
		}
		else
		{
			ImGui::Text("Particle transparency:");
			ImGui::DragFloat("##alpha", &scene->particleGenerator.fixedColorAlpha, 0.1f);
		}

		ImGui::End();
	}
}

void ParticleSystemLauncher::fixedUpdateGame(float deltaTime)
{
	scene->fixedUpdate(deltaTime);
}

void ParticleSystemLauncher::updateGame(float deltaTime)
{
	scene->update(deltaTime);
}

void ParticleSystemLauncher::updateScreen()
{
	ImGui::Render();
#ifdef __EMSCRIPTEN__
	emscripten_get_canvas_element_size("#canvas", &display_w, &display_h);
#else
	glfwGetFramebufferSize(window, &display_w, &display_h);
#endif
	scene->updateProjectionMatrix(display_w, display_h);
	glViewport(0, 0, display_w, display_h);
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene->render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}

	glfwSwapBuffers(window);
}
*/
