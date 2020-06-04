/*
* Copyright (C) 2020 Nick Naumenko (https://gitlab.com/nnaumenko,
* https:://github.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"

#include <iostream>
#include <fstream>

int main(int argc, char ** argv) {
	::testing::InitGoogleTest(&argc, argv);
    const auto testResult = RUN_ALL_TESTS();
    return(testResult);
}
