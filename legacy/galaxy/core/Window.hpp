///
			/// Prepare renderers to render.
			///
			void begin();

			///
			/// Draw renderer contents to screen.
			///
			void end();

			

			///
			/// \brief See if a key is being held down.
			///
			/// This will pick up repeated events.
			///
			/// \param key Key to check.
			///
			/// \return True if key is currently down.
			///
			[[nodiscard]] const bool key_down(input::Keys key) noexcept;

			///
			/// Check if a key was pressed once.
			///
			/// \param key Key to check.
			///
			/// \return True if key was pressed once.
			///
			[[nodiscard]] const bool key_pressed(input::Keys key) noexcept;

			///
			/// Check if a mouse button was pressed.
			///
			/// \param mouse_button mouse button keycode.
			///
			/// \return True if a press occured.
			///
			[[nodiscard]] const bool mouse_button_pressed(input::MouseButtons mouse_button) noexcept;

			///
			/// Check if a mouse button was released.
			///
			/// \param mouse_button mouse button keycode.
			///
			/// \return True if a release occured.
			///
			[[nodiscard]] const bool mouse_button_released(input::MouseButtons mouse_button) noexcept;

			

			///
			/// Get mouse wheel delta.
			///
			/// \return Mouse scroll wheel delta.
			///
			[[nodiscard]] const double get_scroll_delta() noexcept;

			///
			/// Get current cursor position.
			///
			/// \return Returns cursor position as a glm::vec2.
			///
			[[nodiscard]] glm::vec2 get_cursor_pos() noexcept;

			

			

			///
			/// Get cursor size.
			///
			/// \return Const glm::vec2.
			///
			[[nodiscard]] const glm::vec2& cursor_size() const noexcept;

		private:
			///
			/// Post processor.
			///
			std::unique_ptr<graphics::PostProcessor> m_post_processor;