///
/// UniformBuffer.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_UNIFORMBUFFER_HPP_
#define QUASAR_UNIFORMBUFFER_HPP_

///
/// Core namespace.
///
namespace qs
{
	///
	/// Represents an OpenGL uniform buffer.
	///
	class UniformBuffer final
	{
	public:
		///
		/// Constructor.
		///
		UniformBuffer() noexcept;

		///
		/// Destructor.
		///
		~UniformBuffer() noexcept;

		///
		/// Bind buffer to OpenGL context.
		///
		void bind() noexcept;

	private:
		///
		/// Internal OpenGL handle.
		///
		unsigned int m_id;
	};
}

#endif