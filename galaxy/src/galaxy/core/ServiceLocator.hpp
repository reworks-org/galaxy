///
/// ServiceLocator.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_SERVICELOCATOR_HPP_
#define GALAXY_CORE_SERVICELOCATOR_HPP_

#include <memory>

#include "galaxy/meta/Concepts.hpp"
#include "galaxy/error/Log.hpp"

namespace galaxy
{
	namespace core
	{
		///
		/// Provides access to registered services.
		/// Cannot be created, copied or moved.
		///
		/// \tparam Service A class to be stored as a service for use across the engine.
		///
		template<meta::is_class Service>
		class ServiceLocator final
		{
		public:
			///
			/// Construct a new service.
			///
			/// \tparam Args Packed cnstructor argument types.
			///
			/// \param args Packed constructor argument values.
			///
			/// \return Pointer to new service.
			///
			template<typename... Args>
			[[maybe_unused]] static std::shared_ptr<Service> make(Args&&... args) noexcept;

			///
			/// Get a pointer to the service.
			///
			/// \return Shared pointer to service.
			///
			[[nodiscard]] static std::shared_ptr<Service> ptr() noexcept;

			///
			/// Get a reference to the service.
			///
			/// \return Reference to service.
			///
			[[nodiscard]] static Service& ref();

			///
			/// \brief Delete the service and frees up the memory.
			///
			/// Sets the service to nullptr.
			///
			static void del();

		private:
			///
			/// Constructor.
			///
			ServiceLocator() = delete;

			///
			/// Destructor.
			///
			~ServiceLocator() = delete;

			///
			/// Copy constructor.
			///
			ServiceLocator(const ServiceLocator&) = delete;

			///
			/// Move constructor.
			///
			ServiceLocator(ServiceLocator&&) = delete;

			///
			/// Copy assignment operator.
			///
			ServiceLocator& operator=(const ServiceLocator&) = delete;

			///
			/// Move assignment operator.
			///
			ServiceLocator& operator=(ServiceLocator&&) = delete;

		private:
			///
			/// Main service pointer.
			///
			inline static std::shared_ptr<Service> sm_service = nullptr;
		};

		template<meta::is_class Service>
		template<typename... Args>
		inline std::shared_ptr<Service> ServiceLocator<Service>::make(Args&&... args) noexcept
		{
			sm_service = std::make_shared<Service>(std::forward<Args>(args)...);
			return sm_service;
		}

		template<meta::is_class Service>
		inline std::shared_ptr<Service> ServiceLocator<Service>::ptr() noexcept
		{
			return sm_service;
		}

		template<meta::is_class Service>
		inline Service& ServiceLocator<Service>::ref()
		{
			if (!sm_service)
			{
				GALAXY_LOG(GALAXY_FATAL, "Attempted to access undefined reference for service: {0}.", typeid(Service).name());
			}

			return *sm_service;
		}

		template<meta::is_class Service>
		inline void ServiceLocator<Service>::del()
		{
			sm_service.reset();
			sm_service = nullptr;
		}
	} // namespace core
} // namespace galaxy

#endif