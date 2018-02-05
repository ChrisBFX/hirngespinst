cmake_minimum_required(VERSION 3.2)
PROJECT(hirngespinst CXX)


if (MSVC)
	# multiprocess build + bigobj linker
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /bigobj")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()


if (DEFINED EMSCRIPTEN)
 # if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-g)
    add_definitions(-g4 -s ALLOW_MEMORY_GROWTH=1 -s EMULATED_FUNCTION_POINTERS=1 -s NO_EXIT_RUNTIME=1 -s DEMANGLE_SUPPORT=1) # Generates emscripten C++ sourcemaps
#  endif()
endif()

file(GLOB_RECURSE PROJ_INCLUDES include/*.h)
file(GLOB_RECURSE PROJ_SRC src/*.cpp)

set(Boost_USE_STATIC_RUNTIME    OFF)
set(Boost_USE_STATIC_LIBS        ON)
#find_package(Boost 1.54 REQUIRED COMPONENTS system)

if (MSVC)
    add_definitions (-DBOOST_ALL_NO_LIB) # disable autolinking
endif()

set(GSL_INCLUDE_DIRS "C:\\program files\\GSL\\include" CACHE PATH "Path to core guideline library")

include_directories(include ${GSL_INCLUDE_DIRS})# ${Boost_INCLUDE_DIRS}

find_package(js-bind REQUIRED)

#add_library(ui STATIC ${PROJ_SRC} ${PROJ_INCLUDES})
add_executable(hirngespinst ${PROJ_SRC} ${PROJ_INCLUDES})
#target_include_directories(ui PUBLIC
#    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#    $<INSTALL_INTERFACE:include>)
#target_link_libraries(ui ${Boost_LIBRARIES})

#add_subdirectory(test)
target_link_libraries(hirngespinst js-bind::js-bind)

if (DEFINED EMSCRIPTEN)
  set_target_properties(hirngespinst
    PROPERTIES SUFFIX ".html"
    LINK_FLAGS " --bind -s NO_EXIT_RUNTIME=1 -s ALLOW_MEMORY_GROWTH=1 -s EMULATED_FUNCTION_POINTERS=1 -s DEMANGLE_SUPPORT=1")
  em_link_js_library(cxx2js-client ${libraryJsFiles})
  # add -O2 to minify
endif (DEFINED EMSCRIPTEN)

option(Build_Examples OFF "Build examples")