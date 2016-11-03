//
//  Types.hpp
//  REngine3
//
//  Created by reworks on 18/07/2016.
//  Copyright © 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_TYPES_HPP_
#define RENGINE3_TYPES_HPP_

#include <SFML/Config.hpp>

/// 100 pixels = 0.1m
#define b2RatioSmall 1000.0f

/// 100 pixels = 1m
#define b2RatioMedium 100.0f

/// 100 pixels = 10m
#define b2RatioLarge 10.0f

// For doubles
#define DEGTORAD 0.0174532925199432957
#define RADTODEG 57.295779513082320876

// For floats
#define fDEGTORAD 0.0174532925199432957f
#define fRADTODEG 57.295779513082320876f

// Comparison operators
#define AND &&
#define OR ||
#define GTEQ >=
#define LTEQ <=
#define GT >
#define LT <
#define EQUALS ==
#define IS_ASSIGNED =
#define NOT !

// Exit macros
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#endif