///
/// CPP20.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_CPP20_HPP_
#define PROTOSTAR_CPP20_HPP_

///
/// For when CPP20 is fully supported by compilers.
///
#if !defined(__cpp_constinit)
#define PROTOSTAR_constinit constexpr
#else
#define PROTOSTAR_constinit constinit
#endif

#endif