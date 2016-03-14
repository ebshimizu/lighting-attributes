cmake_minimum_required (VERSION 2.6)

project (LightingAttributes)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../cmake/modules")

# defaulting to release build
IF (NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "Release" CACHE STRING "Type of build (Debug Release RelWithDebInfo MinSizeRel)" FORCE)
ENDIF ()

# Compiler options
IF(APPLE)
    SET(CLANG_FLAGS "-std=c++11 -stdlib=libc++")
    
    # HACK(kayvonf): removing frequently occurring warnings due to clp
    # sloppiness.  If possible, someone should try to surpress these
    # warnings only for the relevant file(s).
    SET(CLANG_FLAGS "${CLANG_FLAGS} -Wno-deprecated-register -Wno-extra-tokens")
    
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CLANG_FLAGS}")
ELSEIF(UNIX OR LINUX)
    SET(GCC_FLAGS "-std=c++11 -pthread -fPIC")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_FLAGS}")
    MESSAGE("Detected platform Unix: Adding -std=c++11 to g++ flags")
ENDIF(APPLE)

# Set source files
set (ATTRIBUTE_SOURCE
  sceneGen.cpp
)

set (FEATURE_SOURCE
  featureGen.cpp
)

set (SAMPLE_SOURCE
  sampleGen.cpp
  sampleTools.h
  sampleTools.cpp
)

set (RENDER_SOURCE
  render.cpp
  sampleTools.h
  sampleTools.cpp
)

find_package(Lumiverse REQUIRED)
include_directories(${LUMIVERSE_INCLUDE_DIR} ${LUMIVERSE_ARNOLD_INCLUDE_DIR} C:/lib/libsvm)
set(ACTIVE_LIBRARIES ${LUMIVERSE_LIBRARIES})

# add_executable (SceneGenerator ${ATTRIBUTE_SOURCE})
# target_link_libraries(SceneGenerator ${ACTIVE_LIBRARIES})

add_executable (Sampler ${SAMPLE_SOURCE})
target_link_libraries(Sampler ${ACTIVE_LIBRARIES})

add_executable (Renderer ${RENDER_SOURCE})
target_link_libraries(Renderer ${ACTIVE_LIBRARIES})

# add_executable (FeatureGenerator ${FEATURE_SOURCE})
# target_link_libraries(FeatureGenerator ${ACTIVE_LIBRARIES})
