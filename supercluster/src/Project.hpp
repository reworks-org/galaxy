///
/// Project.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_PROJECT_HPP_
#define SUPERCLUSTER_PROJECT_HPP_

#include <string_view>

#include <galaxy/fs/Serializable.hpp>

#include "instances/Game.hpp"

using namespace galaxy;

namespace sc
{
	///
	/// Project data for editor to load.
	///
	class Project final : public fs::Serializable
	{
	public:
		///
		/// Argument constructor.
		///
		/// \param path Path to file on disk.
		///
		Project(std::string_view path) noexcept;

		///
		/// Export game.
		///
		void export_game();

		///
		/// Set the path to the file on disk.
		///
		/// \param path Path to file on disk.
		///
		void set_path(std::string_view path) noexcept;

		///
		/// Get the path to the file on disk.
		///
		/// \return Path to file on disk as const std::string reference.
		///
		const std::string& get_path() const noexcept;

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
		/// Deleted default constructor.
		///
		Project() = delete;

	public:
		///
		/// Project instance.
		///
		std::shared_ptr<Game> m_instance;

	private:
		///
		/// Path to file on disk.
		///
		std::string m_path;
	};
} // namespace sc

#endif