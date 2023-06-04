///
/// NormalMap.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_NORMALMAP_HPP_
#define GALAXY_GRAPHICS_NORMALMAP_HPP_

namespace galaxy
{
	namespace graphics
	{
		///
		/// Virtual class for allowing a renderable to have normal mapping.
		///
		class NormalMap
		{
		public:
			///
			/// Move constructor.
			///
			NormalMap(NormalMap&&);

			///
			/// Move assignment operator.
			///
			virtual NormalMap& operator=(NormalMap&&);

			///
			/// Destructor.
			///
			virtual ~NormalMap();

			///
			/// Is this sprite normal mapped.
			///
			/// \return True if using a normal map texture.
			///
			[[nodiscard]] bool is_normal_mapped() const;

			///
			/// Get normal map texture id.
			///
			/// \return OpenGL unsigned int handle.
			///
			[[nodiscard]] unsigned int normal_map_texture() const;

		protected:
			///
			/// Constructor.
			///
			NormalMap();

		private:
			///
			/// Copy constructor.
			///
			NormalMap(const NormalMap&) = delete;

			///
			/// Copy assignment operator.
			///
			NormalMap& operator=(const NormalMap&) = delete;

		protected:
			///
			/// Normal map flag.
			///
			bool m_normal_mapped;

			///
			/// Normal map texture handle.
			///
			unsigned int m_nm_texture_handle;
		};
	} // namespace graphics
} // namespace galaxy
#endif