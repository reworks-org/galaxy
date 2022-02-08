void Window::begin()
		{
			RENDERER_2D().prepare();
			m_post_processor->bind();
		}

		void Window::end()
		{
			RENDERER_2D().draw();
			m_post_processor->render_effects();

			// Final Output.
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, m_width, m_height);

			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_post_processor->render_output();

			glfwSwapBuffers(m_window);
		}

		

		const bool Window::key_down(input::Keys key) noexcept
		{
			return glfwGetKey(m_window, m_keyboard.m_keymap[key]) == GLFW_PRESS;
		}

		const bool Window::key_pressed(input::Keys key) noexcept
		{
			bool      res   = false;
			const int state = glfwGetKey(m_window, m_keyboard.m_keymap[key]);
			if (m_keyboard.m_prev_key_states[key] == GLFW_RELEASE && state == GLFW_PRESS)
			{
				res = true;
			}

			m_keyboard.m_prev_key_states[key] = state;
			return res;
		}

		const bool Window::mouse_button_pressed(input::MouseButtons mouse_button) noexcept
		{
			bool res = false;

			const int state = glfwGetMouseButton(m_window, m_mouse.m_mouse_map[mouse_button]);
			if (state == GLFW_PRESS && m_mouse.m_prev_mouse_btn_states[m_mouse.m_mouse_map[mouse_button]] == GLFW_RELEASE)
			{
				res = true;
			}

			m_mouse.m_prev_mouse_btn_states[m_mouse.m_mouse_map[mouse_button]] = state;

			return res;
		}

		const bool Window::mouse_button_released(input::MouseButtons mouse_button) noexcept
		{
			if (glfwGetMouseButton(m_window, m_mouse.m_mouse_map[mouse_button]) == GLFW_RELEASE)
			{
				m_mouse.m_prev_mouse_btn_states[m_mouse.m_mouse_map[mouse_button]] = GLFW_RELEASE;
				return true;
			}

			return false;
		}

		

		const double Window::get_scroll_delta() noexcept
		{
			const double old_delta = m_mouse.m_scroll_delta;
			m_mouse.m_scroll_delta = 0.0;

			return old_delta;
		}

		glm::vec2 Window::get_cursor_pos() noexcept
		{
			glfwGetCursorPos(m_window, &m_cursor.m_pos.x, &m_cursor.m_pos.y);
			return m_cursor.m_pos;
		}

		

		const glm::vec2& Window::cursor_size() const noexcept
		{
			return m_cursor.m_cursor_size;
		}