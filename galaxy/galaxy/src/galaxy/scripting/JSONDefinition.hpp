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
	TextEditor::LanguageDefinition get_json_definition();
} // namespace galaxy

#endif