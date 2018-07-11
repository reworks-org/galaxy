///
/// Serializer.hpp
/// starlight
///
/// Created by reworks on 09/11/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_SERIALIZER_HPP_
#define STARLIGHT_SERIALIZER_HPP_

#include "sl/libs/entt/entity/snapshot.hpp"
#include "sl/libs/entt/entity/registry.hpp"
#include "sl/libs/cereal/archives/json.hpp"

namespace sl
{
	///
	/// Provides interfaces to serialize game components with.
	///
	class Serializer
	{
	public:
		///
		/// Constructor.
		///
		/// \param saveFilePath Path to the save file to read / write from. Expected to end in '/'.
		///
		Serializer(const std::string& saveFilePath);

		///
		/// Default destructor.
		///
		~Serializer() = default;

		///
		/// \brief Create snapshot for game specfic data.
		///
		/// You must override this in a derived class.
		///
		/// \param saveFileName Name of the save file to save to.
		/// \param source Source registry to serialize entities from.
		///
		virtual void createGameSnapshot(const std::string& saveFileName, entt::DefaultRegistry& source) = 0;

		///
		/// \brief Load a snapshot for game specfic data.
		///
		/// You must override this in a derived class.
		///
		/// \param saveFileName Name of the save file to save to.
		/// \param destination Destination registry to deserialize entities to.
		///
		virtual	void loadGameSnapshot(const std::string& saveFileName, entt::DefaultRegistry& destination) = 0;

	protected:
		///
		/// \brief Creates a snapshot of important data in the game engine and writes it out. You need to call this in your user functions!
		///
		///  You need to call this in your user functions! This also calls entt snapshot api for you so no need to worry.
		///
		/// \param oarchive Archive to write save data to.
		/// \param source Source registry to serialize entities from.
		///
		virtual void createFrameworkSnapshot(cereal::JSONOutputArchive& oarchive, entt::DefaultRegistry& source) final;

		///
		/// \brief Loads a snapshot of important data in the game engine and reads it into the game.
		///
		///  You need to call this in your user functions! This also calls entt snapshot api for you so no need to worry.
		///
		/// \param iarchive Archive to read save data from.
		/// \param destination Destination registry to deserialize entities to.
		///
		virtual void loadFrameworkSnapshot(cereal::JSONInputArchive& iarchive, entt::DefaultRegistry& destination) final;

	protected:
		///
		/// Path where the save files are located.
		///
		std::string m_saveFilePath;
	};
}

#endif