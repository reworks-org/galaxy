///
/// Image.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_IMAGE_HPP_
#define GALAXY_IMAGE_HPP_

#include "galaxy/ui/Widget.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Widget specific namespace.
	///
	namespace widget
	{
		///
		/// \brief Simple image widget.
		///
		/// ANY FUNCTIONS YOU BIND TO AN EVENT ARE NOT CALLED ON THE MAIN THREAD.
		/// DO NOT CALL GL CODE IN EVENT FUNCTIONS.
		///
		class Image : public galaxy::Widget
		{
		public:
			///
			/// constructor.
			///
			Image();

			///
			/// Copy constructor.
			///
			Image(const Image&) = delete;

			///
			/// Move constructor.
			///
			Image(Image&&) = default;

			///
			/// Copy assignment operator.
			///
			Image& operator=(const Image&) = delete;

			///
			/// Move assignment operator.
			///
			Image& operator=(Image&&) = default;

			///
			/// Destructor.
			///
			virtual ~Image() = default;

			///
			/// Use theme texture from atlas.
			///
			/// \param name Name of the texture to use.
			///
			void create_from_atlas(std::string_view name);

			///
			/// \brief Update widget logic.
			///
			/// YOU MUST NOT CALL ANY GL CODE FROM THIS FUNCTION. THIS FUNCTION IS CALLED FROM A SEPERATE THREAD.
			///
			void update() override;

			///
			/// \brief Render the widget.
			///
			/// THIS FUNCTION IS CALLED ON THE MAIN THREAD. PUT YOUR GL CODE HERE.
			///
			/// \param camera View camera to use when rendering.
			///
			void render(qs::Camera& camera) override;
		};
	} // namespace widget
} // namespace galaxy

#endif