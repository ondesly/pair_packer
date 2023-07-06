#
#  CMakeLists.txt
#  pair_packer
#
#  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 05.07.2023.
#  Copyright © 2023 Dmitrii Torkhov. All rights reserved.
#

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(pair_packer
        VERSION 1.0.3
        LANGUAGES CXX)

#########
# Build #
#########

add_subdirectory(src)

###########
# Testing #
###########

enable_testing()

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(tests)
endif ()
