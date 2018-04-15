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

namespace sl
{
	class Serializer
	{
	public:
		///
		/// Constructor.
		///
		/// \param saveFilePath Path to the save file to read / write from.
		///
		Serializer(const std::string& saveFilePath) noexcept;

		///
		/// Default destructor.
		///
		~Serializer() = default;

		///
		/// \brief Create snapshot for game specfic data.
		///
		/// You must override this in a derived class.
		///
		virtual void createGameSnapshot() = 0;

		///
		/// \brief Load a snapshot for game specfic data.
		///
		/// You must override this in a derived class.
		///
		virtual	void loadGameSnapshot() = 0;

	protected:
		///
		/// \brief Creates a snapshot of important data in the game engine and writes it out. You need to call this in your user functions!
		///
		///  You need to call this in your user functions! This also calls entt snapshot api for you so no need to worry.
		///
		virtual void createFrameworkSnapshot() final;

		///
		/// \brief Loads a snapshot of important data in the game engine and reads it into the game.
		///
		///  You need to call this in your user functions! This also calls entt snapshot api for you so no need to worry.
		///
		virtual void loadFrameworkSnapshot() final;

	protected:
		std::string m_saveFilePath;
	};
}

#endif