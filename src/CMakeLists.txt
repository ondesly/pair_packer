#
#  CMakeLists.txt
#  pair_packer
#
#  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 05.07.2023.
#  Copyright © 2023 Dmitrii Torkhov. All rights reserved.
#

add_library(${PROJECT_NAME} INTERFACE
        pair_packer/packer.h)

target_include_directories(${PROJECT_NAME} INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)
