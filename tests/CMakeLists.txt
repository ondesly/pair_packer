#
#  CMakeLists.txt
#  pair_packer
#
#  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 05.07.2023.
#  Copyright © 2023 Dmitrii Torkhov. All rights reserved.
#

project(tests)

#

function(add name)
    add_executable(${name} ${name}.cpp)

    target_link_libraries(${name} PRIVATE pair_packer)

    set_target_properties(${name} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)

    add_test(NAME ${name} COMMAND ${name})
endfunction()

#

add("test_packer")
