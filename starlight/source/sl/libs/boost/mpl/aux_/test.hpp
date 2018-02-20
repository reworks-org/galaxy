
#ifndef BOOST_MPL_AUX_TEST_HPP_INCLUDED
#define BOOST_MPL_AUX_TEST_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2002-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include <sl/libs/boost/mpl/aux_/test/test_case.hpp>
#include <sl/libs/boost/mpl/aux_/test/data.hpp>
#include <sl/libs/boost/mpl/aux_/test/assert.hpp>
#include <sl/libs/boost/detail/lightweight_test.hpp>

#include <sl/libs/boost/type_traits/is_same.hpp>

int main()
{
    return boost::report_errors();
}

using namespace boost;
using namespace mpl;

#endif // BOOST_MPL_AUX_TEST_HPP_INCLUDED
