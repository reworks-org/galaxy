///
/// GUI.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <qs/core/Renderer.hpp>

#include "galaxy/res/ShaderBook.hpp"

#include "GUI.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	GUI::GUI() noexcept
		:m_isDestroyed(false), m_counter(0), m_theme(nullptr), m_dt(nullptr)
	{
	}

	GUI::~GUI() noexcept
	{
		if (!m_isDestroyed)
		{
			destroy();
		}
	}

	void GUI::construct(protostar::ProtectedDouble* dt, protostar::ThreadPool* pool) noexcept
	{
		m_dt = dt;

		m_mainLoop.set([&](protostar::ProtectedBool* threadPoolFinished) noexcept
		{
			while (m_running.get() && threadPoolFinished->get())
			{
				processEvents();
				update(m_dt);
			}
		});

		m_visible.set(true);
		m_running.set(true);

		pool->queue(this->getTask());
	}

	void GUI::render(qs::Camera& camera, const unsigned int shader) noexcept
	{
		if (m_visible.get())
		{
			auto* renderer = m_theme->getRenderer();
			auto* shaderPtr = m_theme->get<ShaderBook>(shader);
			auto* batch = m_theme->getBatch();

			shaderPtr->bind();
			shaderPtr->setUniform<glm::mat4>("u_cameraProj", camera.getProj());
			shaderPtr->setUniform<glm::mat4>("u_cameraView", camera.getTransformation());
			
			batch->update();
			batch->bind();
			
			renderer->drawSpriteBatch(*m_theme->getBatch(), *shaderPtr);

			std::lock_guard<std::mutex> l_lock(m_widgetMutex);
			for (auto&& widget : m_widgets)
			{
				widget->activate();
				widget->render(camera);
			}
		}
	}

	Theme* GUI::addTheme() noexcept
	{
		m_theme = std::make_unique<Theme>();
		return m_theme.get();
	}

	void GUI::remove(const unsigned int id) noexcept
	{
		std::lock_guard<std::mutex> l_lock(m_widgetMutex);

		if (id >= m_widgets.size())
		{
			PL_LOG(PL_ERROR, "Invalid widget id.");
		}
		else
		{
			// Don't erase because that will mess up ordering.
			m_widgets[id].reset();
			m_widgets[id] = nullptr;

			m_free.push_back(id);
		}
	}

	void GUI::destroy() noexcept
	{
		m_running.set(false);
		m_mainLoop.waitUntilFinished();

		{
			std::lock_guard<std::mutex> l_lock(m_widgetMutex);

			for (auto&& widget : m_widgets)
			{
				widget.reset();
			}

			m_widgets.clear();
		}

		m_isDestroyed = true;
		m_dt = nullptr;
	}

	void GUI::setVisibility(const bool isVisible) noexcept
	{
		m_visible.set(isVisible);
	}

	protostar::Task* GUI::getTask() noexcept
	{
		return &m_mainLoop;
	}

	void GUI::processEvents() noexcept
	{
		if (m_visible.get())
		{
			std::lock_guard<std::mutex> l_lock(m_eventMutex);
			m_GUIEventManager.trigger();
		}
	}

	void GUI::update(protostar::ProtectedDouble* dt) noexcept
	{
		// Ok to pass pointer here without mutex since deltaTime value can only
		// be accessed through a mutex protected get().
		if (m_visible.get())
		{
			std::lock_guard<std::mutex> l_lock(m_widgetMutex);
			for (auto&& widget : m_widgets)
			{
				widget->update(dt);
			}
		}
	}
}