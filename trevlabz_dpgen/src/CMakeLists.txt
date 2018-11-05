if(MSVC)
    set( CMAKE_CXX_FLAGS "-W4" )
    set(CMAKE_DEBUG_POSTFIX "d")
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
else()
    add_definitions(-Wall -g)
    link_libraries(m)
endif()

set(CMAKE_CXX_FLAGS "-std=c++11")

file( GLOB SRCS *.cpp *.h )

add_executable( dpgen ${SRCS} )
