///
/// Error.hpp
/// Quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_ERROR_HPP_
#define QUASAR_ERROR_HPP_

#include <string>
#include <vector>

namespace qs
{
	///
	/// Allows for Quasar to set an error message in a way that allows for other libraries to access it easily.
	///
	class Error
	{
	public:
		///
		/// Resets current error message and clears history. 
		///
		~Error() noexcept;

		///
		/// Set a new Error Message.
		///
		/// \param message Message to set the current error message to.
		///
		void setError(const std::string& message) noexcept;

		///
		/// Gets the current error message.
		///
		/// \return Returns const reference std::string of current error message.
		///
		const std::string& current() const noexcept;

		///
		/// Retrieve all error messages.
		///
		/// \return Returns const reference of history as a std::vector<std::string>.
		///
		const std::vector<std::string>& getHistory() const noexcept;

		///
		/// Resets current error mesage.
		/// Does not reset history.
		///
		void clear() noexcept;

	private:
		///
		/// Private Default Constructor.
		/// Ensures class can't be constructed.
		///
		explicit Error() noexcept;

	public:
		///
		/// Static handle to error class.
		///
		static Error handle;

	private:
		///
		/// Contains the current error message.
		///
		std::string m_currentError;

		///
		/// Contains a history of all error messages.
		///
		std::vector<std::string> m_history;
	};
}

#endif