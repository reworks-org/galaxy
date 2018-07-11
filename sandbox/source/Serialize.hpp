///
/// Serialize.hpp
/// sandbox
///
/// Created by reworks on 20/05/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef SANDBOX_SERIALIZE_HPP_
#define SANDBOX_SERIALIZE_HPP_

#include <sl/fs/Serializer.hpp>

// uses std::ofstream / std::ifstream for file saving / loading.
class Serialize : public sl::Serializer
{
public:
	///
	/// Constructor.
	///
	/// \param path Expected to end in '/'.
	///
	Serialize(const std::string& path);

	void createGameSnapshot(const std::string& saveFileName, entt::DefaultRegistry& source) override final;
	void loadGameSnapshot(const std::string& saveFileName, entt::DefaultRegistry& destination) override final;
};

#endif