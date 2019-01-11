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

#include "sl/math/Rect.hpp"
#include "sl/math/Vector2.hpp"
#include "sl/math/Vector3.hpp"
#include "sl/math/Vector4.hpp"
#include "sl/core/StateMachine.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/libs/loguru/loguru.hpp"
#include "sl/libs/cereal/archives/json.hpp"
#include "sl/components/PhysicsComponent.hpp"

namespace sl
{
	///
	/// A template to hold a parameter pack.
	/// Thanks to: https://stackoverflow.com/a/52561399
	///
	template<typename...>
	struct Typelist
	{
	};

	///
	/// Declaration of master Serializer class template.
	/// Thanks to: https://stackoverflow.com/a/52561399
	///
	template<typename TypeListOne, typename TypeListTwo>
	struct Serializer;

	///
	/// Class used to serialize game data.
	/// Template parameters are types of entt components and tags to seralize, using template shenanigans.
	/// Thanks to: https://stackoverflow.com/a/52561399
	///
	template<typename... Components, typename... Tags>
	class Serializer<Typelist<Components...>, Typelist<Tags...>> final
	{
	public:
		///
		/// \brief Default constructor.
		///
		/// Save folder defaults to "saves/".
		///
		Serializer();

		///
		/// Path constructor.
		///
		/// \param saveFolder Path to the folder containing save files to read / write from. Expected to end in '/'.
		///
		Serializer(const std::string& saveFolder);

		///
		/// Destructor.
		///
		~Serializer() noexcept = default;

		///
		/// Serialize all data to a file in the save folder.
		///
		/// \param file Save file to write to.
		/// \param registry Entt Registry to seralize.
		///
		void save(const std::string& file, entt::DefaultRegistry& registry);

		///
		/// Deserialize all data to a file in the save folder.
		///
		/// \param file Save file to load.
		/// \param registry Entt Registry to seralize.
		///
		void load(const std::string& file, entt::DefaultRegistry& registry);

		///
		/// \brief Set a lambda to call when seralizing, containing game specific data to be serialized.
		///
		/// Ensure that you seralize in the same order you deseralize! See cereal for a guide to seralizing.
		///
		/// \param callback Lambda function.
		///
		void setSaveCallback(std::function<void(cereal::JSONOutputArchive&)>& callback);

		///
		/// \brief Set a lambda to call when deseralizing, containing game specific data to be deserialized.
		///
		/// Ensure that you deseralize in the same order you seralize! See cereal for a guide to deseralizing.
		///
		/// \param callback Lambda function.
		///
		void setLoadCallback(std::function<void(cereal::JSONInputArchive&)>& callback);

	private:
		///
		///	Seralize entt data.
		///
		/// \param oarchive Cereal JSON output archive to serailize to.
		/// \param registry Entt Registry to seralize.
		///
		void saveEntt(cereal::JSONOutputArchive& oarchive, entt::DefaultRegistry& registry);

		///
		/// Deserialize entt data.
		///
		/// \param iarchive Cereal JSON input archive to deserailize to.
		/// \param registry Entt Registry to deseralize.
		///
		void loadEntt(cereal::JSONInputArchive& iarchive, entt::DefaultRegistry& registry);
		
	private:
		///
		/// Path where the save files are located.
		///
		std::string m_saveFolder;

		///
		/// User callback for seralizing game specific data.
		///
		std::function<void(cereal::JSONOutputArchive&)> m_saveCallback;

		///
		/// User callback for deseralizing game specific data.
		///
		std::function<void(cereal::JSONInputArchive&)> m_loadCallback;
	};

	template<typename... Components, typename... Tags>
	inline Serializer<Typelist<Components...>, Typelist<Tags...>>::Serializer()
		:m_saveFolder("saves/")
	{
	}

	template<typename... Components, typename... Tags>
	inline Serializer<Typelist<Components...>, Typelist<Tags...>>::Serializer(const std::string& saveFolder)
		:m_saveFolder(saveFolder)
	{
	}

	template<typename... Components, typename... Tags>
	inline void Serializer<Typelist<Components...>, Typelist<Tags...>>::save(const std::string& file, entt::DefaultRegistry& registry)
	{
		std::ofstream output(m_saveFolder + file);
		if (!output)
		{
			LOG_S(ERROR) << "Failed to open save file: " << file;
		}
		else
		{
			{
				// Create output archive in scope since cereal completes the process on scope exit.
				cereal::JSONOutputArchive oa(output);

				// Save all component and tag information, including important data in registry.
				saveEntt(oa, registry);
				
				// Serialize the state machine.
				oa(Locator::stateMachine);

				// User data to seralize.
				m_saveCallback(oa);
			}

			// Check to make sure there are no errors that occured during the writing process.
			if (!output)
			{
				LOG_S(ERROR) << "Failed to write data to save file: " << file;
			}
		}

		output.close();
	}

	template<typename... Components, typename... Tags>
	inline void Serializer<Typelist<Components...>, Typelist<Tags...>>::load(const std::string& file, entt::DefaultRegistry& registry)
	{
		std::ifstream input(m_saveFilePath + saveFileName);
		if (!input)
		{
			LOG_S(ERROR) << "Failed to open save file: " << file;
		}
		else
		{
			{
				// Create input archive in scope since cereal completes the process on scope exit.
				cereal::JSONInputArchive ia(input);

				// Load all component and tag information, including important data in registry.
				loadEntt(ia, registry);

				// Deserialize the state machine.
				ia(Locator::stateMachine);

				// User data to deseralize.
				m_loadCallback(ia);
			}

			// Check to make sure there are no errors that occured during the writing process.
			if (!input)
			{
				LOG_S(ERROR) << "Failed to load data from save file: " << file;
			}

			// Ensure that each physicscomponent now points to its new entity.
			registry.view<sl::PhysicsComponent>().each([&](entt::DefaultRegistry::entity_type entity, sl::PhysicsComponent& pc)
			{
				pc.setFixtureEntity(entity);
			});
		}

		input.close();
	}

	template<typename... Components, typename... Tags>
	inline void Serializer<Typelist<Components...>, Typelist<Tags...>>::setSaveCallback(std::function<void(cereal::JSONOutputArchive&)>& callback)
	{
		m_saveCallback = callback;
	}

	template<typename... Components, typename... Tags>
	inline void Serializer<Typelist<Components...>, Typelist<Tags...>>::setLoadCallback(std::function<void(cereal::JSONInputArchive&)>& callback)
	{
		m_loadCallback = callback;
	}

	template<typename... Components, typename... Tags>
	inline void Serializer<Typelist<Components...>, Typelist<Tags...>>::saveEntt(cereal::JSONOutputArchive& oarchive, entt::DefaultRegistry& registry)
	{
		registry.snapshot().entities(oarchive).destroyed(oarchive).component<Components...>(oarchive).tag<Tags...>(oarchive);
	}

	template<typename... Components, typename... Tags>
	inline void Serializer<Typelist<Components...>, Typelist<Tags...>>::loadEntt(cereal::JSONInputArchive& iarchive, entt::DefaultRegistry& registry)
	{
		entt::ContinuousLoader<entt::DefaultRegistry::entity_type> loader(registry);
		loader.entities(iarchive).destroyed(iarchive).component<Components...>(iarchive).tag<Tags...>(iarchive).orphans().shrink();
	}
}

#endif