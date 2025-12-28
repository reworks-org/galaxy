///
/// Camera.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.hpp"

const constexpr auto identity_matrix = glm::mat4 {1.0f};
const constexpr auto rotation_vec    = glm::vec3 {0.0f, 0.0f, 1.0f};

namespace galaxy
{
	Camera::Camera() noexcept
		: m_allow_rotation {false}
		, m_translation_speed {1.0f}
		, m_rotation_speed {1.0f}
		, m_pos {0.0f, 0.0f}
		, m_rotation {0.0f}
		, m_scale {1.0f, 1.0f}
		, m_origin {0.0f, 0.0f}
		, m_dirty {true}
	{
	}

	Camera::Camera(Camera&& c) noexcept
	{
		this->m_allow_rotation    = c.m_allow_rotation;
		this->m_translation_speed = c.m_translation_speed;
		this->m_rotation_speed    = c.m_rotation_speed;
		this->m_data.m_model_view = std::move(c.m_data.m_model_view);
		this->m_data.m_projection = std::move(c.m_data.m_projection);
		this->m_pos               = std::move(c.m_pos);
		this->m_rotation          = c.m_rotation;
		this->m_scale             = std::move(c.m_scale);
		this->m_origin            = std::move(c.m_origin);
		this->m_dirty             = c.m_dirty;
		this->m_transform         = std::move(c.m_transform);
	}

	Camera& Camera::operator=(Camera&& c) noexcept
	{
		if (this != &c)
		{
			this->m_allow_rotation    = c.m_allow_rotation;
			this->m_translation_speed = c.m_translation_speed;
			this->m_rotation_speed    = c.m_rotation_speed;
			this->m_data.m_model_view = std::move(c.m_data.m_model_view);
			this->m_data.m_projection = std::move(c.m_data.m_projection);
			this->m_pos               = std::move(c.m_pos);
			this->m_rotation          = c.m_rotation;
			this->m_scale             = std::move(c.m_scale);
			this->m_origin            = std::move(c.m_origin);
			this->m_dirty             = c.m_dirty;
			this->m_transform         = std::move(c.m_transform);
		}

		return *this;
	}

	Camera::Camera(const Camera& c) noexcept
	{
		this->m_allow_rotation    = c.m_allow_rotation;
		this->m_translation_speed = c.m_translation_speed;
		this->m_rotation_speed    = c.m_rotation_speed;
		this->m_data.m_model_view = c.m_data.m_model_view;
		this->m_data.m_projection = c.m_data.m_projection;
		this->m_pos               = c.m_pos;
		this->m_rotation          = c.m_rotation;
		this->m_scale             = c.m_scale;
		this->m_origin            = c.m_origin;
		this->m_dirty             = c.m_dirty;
		this->m_transform         = c.m_transform;
	}

	Camera& Camera::operator=(const Camera& c) noexcept
	{
		if (this != &c)
		{
			this->m_allow_rotation    = c.m_allow_rotation;
			this->m_translation_speed = c.m_translation_speed;
			this->m_rotation_speed    = c.m_rotation_speed;
			this->m_data.m_model_view = c.m_data.m_model_view;
			this->m_data.m_projection = c.m_data.m_projection;
			this->m_pos               = c.m_pos;
			this->m_rotation          = c.m_rotation;
			this->m_scale             = c.m_scale;
			this->m_origin            = c.m_origin;
			this->m_dirty             = c.m_dirty;
			this->m_transform         = c.m_transform;
		}

		return *this;
	}

	Camera::~Camera() noexcept
	{
	}

	void Camera::set_projection(const float left, const float right, const float bottom, const float top) noexcept
	{
		// By default sets origin to center of right, bottom.
		m_origin.x = right * 0.5f;
		m_origin.y = bottom * 0.5f;

		m_data.m_projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);

		m_dirty = true;
	}

	void Camera::move(const float x, const float y) noexcept
	{
		m_pos.x += (x * m_translation_speed);
		m_pos.y += (y * m_translation_speed);

		m_dirty = true;
	}

	void Camera::move_x(const float x) noexcept
	{
		m_pos.x += (x * m_translation_speed);
		m_dirty  = true;
	}

	void Camera::move_y(const float y) noexcept
	{
		m_pos.y += (y * m_translation_speed);
		m_dirty  = true;
	}

	void Camera::rotate(const float degrees) noexcept
	{
		m_rotation += (degrees * m_rotation_speed);
		m_rotation  = std::clamp(m_rotation, 0.0f, 360.0f);

		m_dirty = true;
	}

	void Camera::scale(const float scale) noexcept
	{
		m_scale.x = scale;
		m_scale.y = scale;

		m_dirty = true;
	}

	void Camera::set_scale_horizontal(const float x) noexcept
	{
		m_scale.x = x;
		m_dirty   = true;
	}

	void Camera::set_scale_vertical(const float y) noexcept
	{
		m_scale.y = y;
		m_dirty   = true;
	}

	void Camera::set_rotation(const float degrees) noexcept
	{
		m_rotation = std::clamp(degrees, 0.0f, 360.0f);
		m_dirty    = true;
	}

	void Camera::set_positon(const float x, const float y) noexcept
	{
		m_pos.x = x;
		m_pos.y = y;

		m_dirty = true;
	}

	void Camera::set_positon_horizontal(const float x) noexcept
	{
		m_pos.x = x;
		m_dirty = true;
	}

	void Camera::set_positon_vertical(const float y) noexcept
	{
		m_pos.y = y;
		m_dirty = true;
	}

	void Camera::set_origin(const float x, const float y) noexcept
	{
		m_origin.x = x;
		m_origin.y = y;

		m_dirty = true;
	}

	void Camera::reset() noexcept
	{
		m_pos       = {0.0f, 0.0f};
		m_rotation  = 0.0f;
		m_scale     = {1.0f, 1.0f};
		m_transform = {1.0f};

		m_dirty = true;
	}

	const glm::vec2& Camera::get_pos() const noexcept
	{
		return m_pos;
	}

	float Camera::get_rotation() const noexcept
	{
		return m_rotation;
	}

	const glm::vec2& Camera::get_scale() const noexcept
	{
		return m_scale;
	}

	const glm::vec2& Camera::get_origin() const noexcept
	{
		return m_origin;
	}

	glm::mat4& Camera::get_transform() noexcept
	{
		recalculate();
		return m_transform;
	}

	const glm::mat4& Camera::get_model_view() noexcept
	{
		recalculate();
		return m_data.m_model_view;
	}

	const glm::mat4& Camera::get_proj() noexcept
	{
		return m_data.m_projection;
	}

	Camera::Data& Camera::get_data() noexcept
	{
		recalculate();
		return m_data;
	}

	void Camera::recalculate() noexcept
	{
		if (m_dirty)
		{
			const auto origin = glm::vec3 {m_origin, 0.0f};

			auto rotation = glm::translate(identity_matrix, origin);
			rotation      = glm::rotate(rotation, glm::radians(m_rotation), rotation_vec);
			rotation      = glm::translate(rotation, -origin);

			auto scale = glm::translate(identity_matrix, origin);
			scale      = glm::scale(scale, {m_scale, 1.0f});
			scale      = glm::translate(scale, -origin);

			m_transform         = glm::translate(identity_matrix, {m_pos, 0.0f}) * rotation * scale;
			m_data.m_model_view = glm::inverse(m_transform);
			m_dirty             = false;
		}
	}
} // namespace galaxy
