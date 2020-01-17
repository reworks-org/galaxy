///
/// JSONDefinition.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_JSONDEFINITION_HPP_
#define GALAXY_JSONDEFINITION_HPP_

#include <TextEditor.h>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Get a custom language definition for JSON.
	///
	/// \return LanguageDefinition object for the text editor.
	///
	const TextEditor::LanguageDefinition& getJsonDefinition() noexcept;
}

#endif