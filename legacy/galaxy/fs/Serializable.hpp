///
/// Serializable.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_FS_SERIALIZABLE_HPP_
#define GALAXY_FS_SERIALIZABLE_HPP_

#include <nlohmann/json.hpp>

#include "galaxy/error/Log.hpp"

namespace galaxy
{
	namespace fs
	{
		class Serializable;

		///
		/// \brief Contains a pointer to a serializable type.
		///
		/// This is to prevent slicing when parsing to/from json.
		///
		struct SerializableWrapper final
		{
			///
			/// Constructor.
			///
			SerializableWrapper() noexcept;

			///
			/// Destructor.
			///
			~SerializableWrapper() noexcept;

			///
			/// Pointer to object.
			///
			Serializable* m_obj;
		};

		///
		/// \brief Allows a class to be serialized.
		///
		/// A class wanting to gain serializability must inherit from this class and implement the interface.
		///
		class Serializable
		{
		public:
			///
			/// Default virtual destructor.
			///
			virtual ~Serializable() = default;

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to be serialized.
			///
			[[nodiscard]] virtual nlohmann::json serialize() = 0;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			virtual void deserialize(const nlohmann::json& json) = 0;

		protected:
			///
			/// Arugment constructor.
			///
			/// \param s Should be a 'this' pointer.
			///
			Serializable(Serializable* s) noexcept;

		private:
			///
			/// Constructor.
			///
			Serializable() noexcept;

		private:
			///
			/// Wraps pointer to prevent slicing.
			/// Automatically assigned during constructor call.
			///
			SerializableWrapper m_wrapper;
		};

		inline void to_json(nlohmann::json& json, const SerializableWrapper& wrapper)
		{
			if (wrapper.m_obj != nullptr)
			{
				json = wrapper.m_obj->serialize();
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to serialize nullptr object.");
			}
		}

		inline void from_json(const nlohmann::json& json, SerializableWrapper& wrapper)
		{
			if (wrapper.m_obj != nullptr)
			{
				wrapper.m_obj->deserialize(json);
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to deserialize nullptr object.");
			}
		}
	} // namespace fs
} // namespace galaxy

#endif