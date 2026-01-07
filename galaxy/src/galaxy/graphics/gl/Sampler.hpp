///
/// Sampler.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_GL_SAMPLER_HPP_
#define GALAXY_GRAPHICS_GL_SAMPLER_HPP_

namespace galaxy
{
	///
	/// Texture sampler definitions.
	///
	class Sampler final
	{
	public:
		///
		/// Constructor.
		///
		Sampler() noexcept;

		///
		/// Move constructor.
		///
		Sampler(Sampler&&) noexcept;

		///
		/// Move assignment operator.
		///
		Sampler& operator=(Sampler&&) noexcept;

		///
		/// Destructor.
		///
		~Sampler() noexcept;

		///
		/// Set a sampler field.
		///
		/// \param param Texture parameter sampler name.
		/// \param value Value of the parameter.
		///
		void set(const unsigned int param, const int value) const;

		///
		/// Set a float sampler field.
		///
		/// \param param Texture parameter sampler name.
		/// \param value Value of the parameter.
		///
		void setf(const unsigned int param, const float value) const;

		///
		/// \brief Bind sampler.
		///
		/// Not required with bindless textures.
		///
		/// \param texture_unit Texture at the texture unit to bind to.
		///
		void bind(const unsigned int texture_unit) const;

		///
		/// \brief Unbind sampler.
		///
		/// Not required with bindless textures.
		///
		/// \param texture_unit Texture at the texture unit to unbind from.
		///
		void unbind(const unsigned int texture_unit) const;

		///
		/// Get program id.
		///
		/// \return unsigned int.
		///
		[[nodiscard]]
		unsigned int id() const noexcept;

	private:
		///
		/// Copy constructor.
		///
		Sampler(const Sampler&) = delete;

		///
		/// Copy assignment operator.
		///
		Sampler& operator=(const Sampler&) = delete;

	private:
		///
		/// OpenGL handle.
		///
		unsigned int m_id;
	};
} // namespace galaxy

#endif
