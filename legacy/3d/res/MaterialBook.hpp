///
/// MaterialBook.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RES_MATERIALBOOK_HPP_
#define GALAXY_RES_MATERIALBOOK_HPP_

#include "galaxy/graphics/light/Material.hpp"
#include "galaxy/res/ResourceCache.hpp"

namespace galaxy
{
	namespace res
	{
		///
		/// Resource manager for materials.
		///
		class MaterialBook final : public ResourceCache<light::Material>
		{
		public:
			///
			/// Constructor.
			///
			MaterialBook() noexcept = default;

			///
			/// Destructor.
			///
			virtual ~MaterialBook() noexcept;

			///
			/// Clean up.
			///
			void clear() noexcept override;

		private:
			///
			/// Copy constructor.
			///
			MaterialBook(const MaterialBook&) = delete;

			///
			/// Move constructor.
			///
			MaterialBook(MaterialBook&&) = delete;

			///
			/// Copy assignment operator.
			///
			MaterialBook& operator=(const MaterialBook&) = delete;

			///
			/// Move assignment operator.
			///
			MaterialBook& operator=(MaterialBook&&) = delete;
		};
	} // namespace res
} // namespace galaxy

#endif