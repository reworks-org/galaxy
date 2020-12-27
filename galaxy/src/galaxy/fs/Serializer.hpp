///
/// Serializer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_FS_SERIALIZER_HPP_
#define GALAXY_FS_SERIALIZER_HPP_

#include <robin_hood.h>

#include "galaxy/fs/Serializable.hpp"

namespace galaxy
{
	namespace fs
	{
		///
		/// Class used to serialize game data.
		///
		class Serializer final
		{
		public:
			///
			/// \brief Default constructor.
			///
			/// Serialize folder defaults to "saves/".
			///
			Serializer();

			///
			/// Destructor.
			///
			~Serializer();

			///
			/// Add a pointer to a class that contains a serialize() function.
			///
			/// \param s Pointer to class that can be serialized.
			///
			unsigned int register_obj(Serializable* s);

			///
			/// Remove an object from the serializer.
			///
			/// \param id Object serializer id.
			///
			void remove(const unsigned int id);

			///
			/// Serialize all stored objects.
			///
			/// \param file Path to a json file, including the file.
			///
			void serialize(std::string_view file);

			///
			/// Deserialize a json file.
			///
			/// \param file Path to a json file, including the file.
			///
			void deserialize(std::string_view file);

		private:
			///
			/// ID counter.
			///
			long m_counter;

			///
			/// Stores pointers to serializable classes.
			///
			robin_hood::unordered_map<long, Serializable*> m_objects;
		};
	} // namespace fs
} // namespace galaxy

#endif