///
/// Flag.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_FLAG_HPP_
#define GALAXY_COMPONENTS_FLAG_HPP_

#include <bitset>

#include <entt/fwd.hpp>

#include "galaxy/meta/Concepts.hpp"
#include "galaxy/fs/Serializable.hpp"
#include "galaxy/utils/Globals.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Flag an entity.
		///
		class Flag final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Flag();

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Flag(const nlohmann::json& json);

			///
			/// Pointer constructor.
			///
			/// \param ptr Data to COPY into this component.
			///
			Flag(Flag* ptr);

			///
			/// Move constructor.
			///
			Flag(Flag&&);

			///
			/// Move assignment operator.
			///
			Flag& operator=(Flag&&);

			///
			/// Destructor.
			///
			virtual ~Flag();

			///
			/// Set a flag on an entity.
			///
			/// \tparam _Flag Flag to set.
			///
			/// \param entity Entity to set flag on.
			///
			template<meta::is_bitset_flag _Flag>
			void set_flag();

			///
			/// Check flag value on an entity.
			///
			/// \tparam _Flag Flag to set.
			///
			/// \param entity Entity to check flag.
			///
			template<meta::is_bitset_flag _Flag>
			[[nodiscard]] bool is_flag_set();

			///
			/// Unset a flag on an entity.
			///
			/// \tparam _Flag Flag to set.
			///
			/// \param entity Entity to unset flag on.
			///
			template<meta::is_bitset_flag _Flag>
			void unset_flag();

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to be serialized.
			///
			[[nodiscard]] nlohmann::json serialize() override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			void deserialize(const nlohmann::json& json) override;

		private:
			///
			/// Copy assignment operator.
			///
			Flag& operator=(const Flag&) = delete;

			///
			/// Copy constructor.
			///
			Flag(const Flag&) = delete;

		private:
			///
			/// Stores entity flags.
			///
			std::bitset<GALAXY_FLAG_BITSET_COUNT> m_flags;
		};

		template<meta::is_bitset_flag _Flag>
		inline void Flag::set_flag()
		{
			m_flags.set(_Flag::value);
		}

		template<meta::is_bitset_flag _Flag>
		inline bool Flag::is_flag_set()
		{
			return m_flags.test(_Flag::value);
		}

		template<meta::is_bitset_flag _Flag>
		inline void Flag::unset_flag()
		{
			m_flags.reset(_Flag::value);
		}
	} // namespace components
} // namespace galaxy

#endif