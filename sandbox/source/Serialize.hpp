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

class Serialize : public sl::Serializer
{
public:
	Serialize(const std::string& path);

	void createGameSnapshot(entt::DefaultRegistry& source) override final;
	void loadGameSnapshot(entt::DefaultRegistry& destination) override final;
};

#endif