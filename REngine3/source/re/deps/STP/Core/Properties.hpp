////////////////////////////////////////////////////////////
//
// The MIT License (MIT)
//
// STP - SFML TMX Parser
// Copyright (c) 2013-2014 Manuel Sabogal
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////////

#ifndef STP_PROPERTIES_HPP
#define STP_PROPERTIES_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <string>
#include <unordered_map>

#include "../Config.hpp"

namespace tmx {

////////////////////////////////////////////////////////////
/// \brief Base class to manage the Tiled properties of an element
///
////////////////////////////////////////////////////////////
class STP_API Properties {
 public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Properties();

    ////////////////////////////////////////////////////////////
    /// \brief Add new property.
    ///
    /// \param name  The name of the property
    /// \param value The value of the property
    ///
    ////////////////////////////////////////////////////////////
    void AddProperty(const std::string& name, const std::string& value);

    ////////////////////////////////////////////////////////////
    /// \brief Return a property value given a name
    ///
    /// \param name The name of the property
    ///
    /// \return Reference to the property value
    ///
    ////////////////////////////////////////////////////////////
    std::string& GetPropertyValue(const std::string& name);

 protected:
    /// \brief Hash table to store the values
    std::unordered_map<std::string, std::string> properties_;
};

}  // namespace tmx

#endif  // STP_PROPERTIES_HPP
