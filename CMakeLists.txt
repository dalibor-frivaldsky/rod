## Copyright Dalibor Frivaldsky 2017
## Distributed under the Boost Software License, Version 1.0.
## See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt

cmake_minimum_required(VERSION 3.1)

project(rod)


##
## Version
##

set(ROD_VERSION_MAJOR 0)
set(ROD_VERSION_MINOR 1)
set(ROD_VERSION_PATCH 0)
set(ROD_VERSION "${ROD_VERSION_MAJOR}.${ROD_VERSION_MINOR}.${ROD_VERSION_PATCH}")


##
## Compiler options
##

set(CMAKE_CXX_COMPILER "g++-7")
set(CMAKE_CXX_FLAGS "-std=c++1z -fconcepts")


##
## Directories
##

set(ROD_TEST_DIR ${PROJECT_SOURCE_DIR}/test)


##
## Dependencies
##

include_directories(
	${PROJECT_SOURCE_DIR}/../../lib/hana/include
	${PROJECT_SOURCE_DIR}/../../lib/callable_traits-1.1.0/include
	${PROJECT_SOURCE_DIR}/../../lib/range-v3/include
)


##
## Rod library
##

add_library(rod INTERFACE)
target_include_directories(rod INTERFACE ${PROJECT_SOURCE_DIR}/include)


##
## Testing
##

enable_testing()

set(ROD_TEST_SOURCES
	${ROD_TEST_DIR}/test_main.cpp
	${ROD_TEST_DIR}/unit/rod/concept/Callable.spec.cpp
	${ROD_TEST_DIR}/unit/rod/concept/CallableWith.spec.cpp
	${ROD_TEST_DIR}/unit/rod/feature/instance/instance.spec.cpp
	${ROD_TEST_DIR}/unit/rod/feature/instance/as.spec.cpp
	${ROD_TEST_DIR}/unit/rod/feature/type/base_of.spec.cpp
	${ROD_TEST_DIR}/unit/rod/feature/type/is.spec.cpp
	${ROD_TEST_DIR}/unit/rod/feature/type/of_base.spec.cpp
	${ROD_TEST_DIR}/unit/rod/feature/type/type.spec.cpp
	${ROD_TEST_DIR}/unit/rod/pipe.spec.cpp
	${ROD_TEST_DIR}/unit/rod/resolve.spec.cpp
	${ROD_TEST_DIR}/unit/rod/source/build/entity.spec.cpp
	${ROD_TEST_DIR}/unit/rod/source/build/tag_matcher.spec.cpp
	${ROD_TEST_DIR}/unit/rod/source/build/with_query.spec.cpp
	${ROD_TEST_DIR}/unit/rod/source/hana_monad.spec.cpp
	${ROD_TEST_DIR}/unit/rod/source/type_index.spec.cpp
	${ROD_TEST_DIR}/integration/rod/feature/instance/instance_as.spec.cpp
	${ROD_TEST_DIR}/integration/rod/source/hana_tuple_resolving.spec.cpp
	${ROD_TEST_DIR}/integration/rod/source/type_index_resolving.spec.cpp
	${ROD_TEST_DIR}/functional/rod/resolve/resolve.api.spec.cpp
)

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

add_executable(rod-test ${ROD_TEST_SOURCES})
target_link_libraries(rod-test rod Catch)
add_test(NAME rod-unit-tests COMMAND rod-test)
	
