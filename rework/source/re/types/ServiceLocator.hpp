///
///  ServiceLocator.hpp
///  rework
///
///  Created by reworks on 17/07/2016.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef REWORK_SERVICELOCATOR_HPP_
#define REWORK_SERVICELOCATOR_HPP_

#include <memory>
#include <utility>

namespace re
{
	template<typename Service>
	class ServiceLocator
	{
		friend Service;
	public:
		///
		/// Destructor.
		///
		virtual ~ServiceLocator() = default;

		///
		/// Make a new service.
		///
		/// \param Args Arguments for service.
		///
		template<typename... Args>
		inline static void make(Args&&... args)
		{
			m_servicePtr = std::make_unique<Service>(std::forward<Args>(args)...);
		}

		///
		/// \brief Delete the service.
		///
		/// Calls the destructor of the service, and sets pointer to nullptr.
		///
		inline static void destroy()
		{
			m_servicePtr.reset();
		}

		///
		/// Retrieves service.
		///
		/// \return Returns a pointer to the service.
		///
		inline static Service* get()
		{
			return m_servicePtr.get();
		}

	private:
		static std::unique_ptr<Service> m_servicePtr;

	private:
		///
		/// Default constructor.
		///
		ServiceLocator() = default;

		///
		/// Copy Constructor.
		/// Deleted.
		///
		ServiceLocator(const ServiceLocator&) = delete;

		///
		/// Move Constructor.
		/// Deleted.
		///
		ServiceLocator(ServiceLocator&&) = delete;
	};
}

#endif