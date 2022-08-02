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
			[[maybe_unused]] static Service& make(Args&&... args) noexcept;

			///
			/// Get a reference to the service.
			///
			/// \return Reference to service.
			///
			[[nodiscard]] static Service& ref();

			///
			/// Check if service exists.
			///
			/// \return True if service has been created.
			///
			[[nodiscard]] static bool empty() noexcept;

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
			inline static std::unique_ptr<Service> m_service = nullptr;
		};

		template<meta::is_class Service>
		template<typename... Args>
		inline Service& ServiceLocator<Service>::make(Args&&... args) noexcept
		{
			m_service = std::make_unique<Service>(std::forward<Args>(args)...);
			return ref();
		}

		template<meta::is_class Service>
		inline Service& ServiceLocator<Service>::ref()
		{
			if (m_service == nullptr)
			{
				GALAXY_LOG(GALAXY_FATAL, "Attempted to access undefined reference for service: {0}.", typeid(Service).name());
			}

			return *m_service;
		}

		template<meta::is_class Service>
		inline bool ServiceLocator<Service>::empty() noexcept
		{
			return m_service == nullptr;
		}

		template<meta::is_class Service>
		inline void ServiceLocator<Service>::del()
		{
			m_service.reset();
			m_service = nullptr;
		}
	} // namespace core
} // namespace galaxy

#endif