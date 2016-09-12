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


////////////////////////////////////////////////////////////
/// Configuration header of the library
////////////////////////////////////////////////////////////

#include <cstdint>

#ifndef STP_CONFIG_HPP
#define STP_CONFIG_HPP

#define STP_STATIC

// Define DLL import/export macros (only Windows, and only dynamic configuration)
#if (defined(_WIN32) || defined(__WIN32__)) && !defined(STP_STATIC)

    // Export dynamic link library (from DLL side)
    #ifdef STP_EXPORTS
        #define STP_API __declspec(dllexport)

    // Import dynamic link library (from client side)
    #else
        #define STP_API __declspec(dllimport)

    #endif  // STP_EXPORTS

    // Disable annoying MSVC++ warning
    #ifdef _MSC_VER
        #pragma warning(disable: 4251)
    #endif  // _MSC_VER

// Other platforms don't have DLLs
#else
    #define STP_API

#endif


// Version of the library
#define STP_VERSION_MAJOR 0
#define STP_VERSION_MINOR 1


#endif  // STP_CONFIG_HPP
