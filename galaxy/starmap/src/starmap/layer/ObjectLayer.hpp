///
/// ObjectLayer.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_OBJECTLAYER_HPP_
#define STARMAP_OBJECTLAYER_HPP_

#include "starmap/layer/Layer.hpp"
#include "starmap/types/Object.hpp"

///
/// Core namespace.
///
namespace starmap
{
	///
	/// Specialized type of layer.
	///
	class ObjectLayer final : public starmap::Layer
	{
	public:
		///
		/// \brief Default constructor.
		///
		ObjectLayer() noexcept;

		///
		/// \brief Parse constructor.
		///
		/// Does not call ObjectLayer::parse() you must call that afterwards.
		/// Parses Layer common json.
		///
		/// \param json JSON structure containing chunk array from root map.
		///
		explicit ObjectLayer(const nlohmann::json& json) noexcept;

		///
		/// Destructor.
		///
		~ObjectLayer() noexcept;

		///
		/// Get draw order.
		///
		/// \return Draw order as std::string.
		///
		const std::string& getCompression() const noexcept;

		///
		/// Get objects.
		///
		/// \return Std::vector of objects.
		///
		const auto& getObjects() const noexcept;

	private:
		///
		/// topdown (default) or index.
		///
		std::string m_drawOrder;

		///
		/// Array of objects.
		///
		std::vector<starmap::Object> m_objects;
	};
}

#endif