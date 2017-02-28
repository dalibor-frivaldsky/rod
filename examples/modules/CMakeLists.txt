## Copyright Dalibor Frivaldsky 2017
## Distributed under the Boost Software License, Version 1.0.
## See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt

cmake_minimum_required(VERSION 3.1)

project(modules)


##
## Compiler options
##

set(CMAKE_CXX_COMPILER "g++-7")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -fconcepts -fPIC")


##
## Dependencies
##

#if(NOT TARGET Catch)
	include(ExternalProject)
	set(CATCH_FETCH
		URL https://github.com/philsquared/Catch/archive/v1.5.9.tar.gz
	)
	ExternalProject_Add(Catch-External
		PREFIX ${CMAKE_BINARY_DIR}/external/Catch
		${CATCH_FETCH}
		CONFIGURE_COMMAND ""
		BUILD_COMMAND ""
		INSTALL_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/external/Catch/src/Catch-External/single_include/catch.hpp
			${CMAKE_BINARY_DIR}/external/Catch/include/catch.hpp
	)
	add_library(Catch INTERFACE)
	add_dependencies(Catch INTERFACE Catch-External)
	target_include_directories(Catch INTERFACE ${CMAKE_BINARY_DIR}/external/Catch/include)
#endif()

#if(NOT TARGET Hana)
	include(ExternalProject)
	set(HANA_FETCH
		URL https://github.com/boostorg/hana/archive/v1.0.2.tar.gz
	)
	ExternalProject_Add(Hana-External
		PREFIX ${CMAKE_BINARY_DIR}/external/Hana
		${HANA_FETCH}
		CONFIGURE_COMMAND ""
		BUILD_COMMAND ""
		INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/external/Hana/src/Hana-External/include
			${CMAKE_BINARY_DIR}/external/Hana/include
	)
	add_library(Hana INTERFACE)
	add_dependencies(Hana INTERFACE Hana-External)
	target_include_directories(Hana INTERFACE ${CMAKE_BINARY_DIR}/external/Hana/include)
#endif()

#if(NOT TARGET Callable-Traits)
	include(ExternalProject)
	set(CALLABLE_TRAITS_FETCH
		URL https://github.com/badair/callable_traits/archive/1.1.1.tar.gz
	)
	ExternalProject_Add(Callable-Traits-External
		PREFIX ${CMAKE_BINARY_DIR}/external/Callable-Traits
		${CALLABLE_TRAITS_FETCH}
		CONFIGURE_COMMAND ""
		BUILD_COMMAND ""
		INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/external/Callable-Traits/src/Callable-Traits-External/include
			${CMAKE_BINARY_DIR}/external/Callable-Traits/include
	)
	add_library(Callable-Traits INTERFACE)
	add_dependencies(Callable-Traits INTERFACE Callable-Traits-External)
	target_include_directories(Callable-Traits INTERFACE ${CMAKE_BINARY_DIR}/external/Callable-Traits/include)
#endif()

#if(NOT TARGET Range-V3)
	include(ExternalProject)
	set(RANGE_V3_FETCH
		GIT_REPOSITORY git@github.com:dalibor-frivaldsky/range-v3.git
		GIT_TAG gcc7-quirks
	)
	ExternalProject_Add(Range-V3-External
		PREFIX ${CMAKE_BINARY_DIR}/external/Range-V3
		${RANGE_V3_FETCH}
		CONFIGURE_COMMAND ""
		UPDATE_COMMAND ""
		BUILD_COMMAND ""
		INSTALL_COMMAND ""
	)
	add_library(Range-V3 INTERFACE)
	add_dependencies(Range-V3 INTERFACE Range-V3-External)
	target_include_directories(Range-V3 INTERFACE ${CMAKE_BINARY_DIR}/external/Range-V3/src/Range-V3-External/include)
#endif()


##
## Rod library
##

add_library(rod INTERFACE)
target_include_directories(rod INTERFACE ${PROJECT_SOURCE_DIR}/../../include)


##
## Example target
##

add_executable(modules ${PROJECT_SOURCE_DIR}/main.cpp)
target_link_libraries(modules rod Catch Hana Callable-Traits Range-V3 -ldl)


add_library(module1 SHARED ${PROJECT_SOURCE_DIR}/module1.cpp)
target_link_libraries(module1 rod Catch Hana Callable-Traits Range-V3)

add_library(module2 SHARED ${PROJECT_SOURCE_DIR}/module2.cpp)
target_link_libraries(module2 rod Catch Hana Callable-Traits Range-V3)
