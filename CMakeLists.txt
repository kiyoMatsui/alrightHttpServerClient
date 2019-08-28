## Project Properties
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(projectName)

SET(APPLICATION_NAME "projectName")
SET(APPLICATION_VERSION_STRING 0.1)

## Project Output Paths
SET(MAINFOLDER ${PROJECT_SOURCE_DIR})
SET(EXECUTABLE_OUTPUT_PATH "${MAINFOLDER}/bin")

## Project Search Paths
LIST(APPEND CMAKE_PREFIX_PATH "${MAINFOLDER}")
SET(CMAKE_MODULE_PATH "${MAINFOLDER}/cmake")
INCLUDE_DIRECTORIES("${MAINFOLDER}/include")

INCLUDE_DIRECTORIES("${MAINFOLDER}/thirdParty/catch") #catch.hpp

## example project Sources
FILE(GLOB_RECURSE EXAMPLE_SRCS01 ${MAINFOLDER}/src/*.cpp )

## Project Dependencies

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ~/boost/include)
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ~/boost/lib)
## Project Dependencies
find_package(Boost REQUIRED COMPONENTS system program_options regex filesystem)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
#INCLUDE_DIRECTORIES(~/boost/include)
#LINK_DIRECTORIES(~/boost/lib)


## Configure Files
FILE(GLOB_RECURSE CONFIGINPUTS1 include/*.h.in.cmake)
LIST(APPEND CONFIGINPUTS "${CONFIGINPUTS1}")
FOREACH(CONFIGINPUT ${CONFIGINPUTS})
    STRING(REPLACE ".h.in.cmake" ".h" CONFIGOUTPUT1 "${CONFIGINPUT}")
    CONFIGURE_FILE("${CONFIGINPUT}" "${CONFIGOUTPUT1}")
ENDFOREACH(CONFIGINPUT ${CONFIGINPUTS})

## Use compiler debugging flags per default
INCLUDE(CxxFlags)

#debug
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -lcrypto -lssl -pthread -std=c++1z")

#release
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -lcrypto -lssl -pthread -std=c++1z")


set(BUILD_TESTING true CACHE BOOL "Enable building of tests.")

## Creating Binaries for examples

SET(PROJECT_LIBS ${CMAKE_DL_LIBS} ${Boost_LIBRARIES})

SET(EXAMPLE1_NAME "example")
SET(EXAMPLE1_BIN ${EXAMPLE1_NAME})
ADD_EXECUTABLE(${EXAMPLE1_BIN} ${MAINFOLDER}/src/main.cpp ${EXAMPLE_SRCS01})

TARGET_LINK_LIBRARIES(${EXAMPLE1_BIN} ${PROJECT_LIBS})

INSTALL(TARGETS ${EXAMPLE_BIN} DESTINATION bin)


macro(create_test TARGET_NAME SOURCE DEPENDENCIES)
    add_executable(${TARGET_NAME} ${SOURCE})
    target_link_libraries(
        ${TARGET_NAME}
        ${DEPENDENCIES}
        catchMain
        )
endmacro()

if (BUILD_TESTING)
    ADD_LIBRARY(catchMain ./tests/00CatchMain.cpp)
    enable_testing()
    create_test(clientTest ./tests/clientTest.cpp ${PROJECT_LIBS})
endif (BUILD_TESTING)
