# ==============================================================================
# HiPoLy CMake File
# ==============================================================================

CMAKE_MINIMUM_REQUIRED(VERSION 3.0 FATAL_ERROR)
#SET(CMAKE_VERBOSE_MAKEFILE ON)

# ==============================================================================
# conditional setup ------------------------------------------------------------
# ==============================================================================

SET(LIBS_STATIC     OFF CACHE BOOL "Should the static version of scimafic library be built?")
SET(LIBS_SHARED     ON  CACHE BOOL "Should the dynamic version of scimafic library be built?")
SET(TRY_QT_LIB      ON  CACHE BOOL "Should the qt lib be used?")

# ==============================================================================
# project setup ----------------------------------------------------------------
# ==============================================================================

# what languages we use ---------------------------
ENABLE_LANGUAGE(C)
ENABLE_LANGUAGE(CXX)

# library version ---------------------------------
SET(HIPOLY_VERSION 1.0.x)

# ==============================================================================
# external dependencies --------------------------------------------------------
# ==============================================================================

IF(NOT DEFINED COMPOSITE_PROJECT)
    SET(DEVELOPMENT_ROOT "$ENV{DEVELOPMENT_ROOT}")
ENDIF(NOT DEFINED COMPOSITE_PROJECT)

IF(NOT DEVELOPMENT_ROOT)
    MESSAGE(FATAL_ERROR "DEVELOPMENT_ROOT variable is not set!")
ENDIF(NOT DEVELOPMENT_ROOT)

# ==============================================================================
# external libraries -----------------------------------------------------------
# ==============================================================================

# boost =======================
FIND_PACKAGE(Boost REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} SYSTEM)

# try to find threads ----------------------------
FIND_PACKAGE(Threads)

IF(CMAKE_USE_PTHREADS_INIT)
    SET(HAVE_PTHREAD_H 1)
    ADD_DEFINITIONS(-DHAVE_PTHREAD_H)
ELSE(CMAKE_USE_PTHREADS_INIT)
    IF(CMAKE_USE_WIN32_THREADS_INIT)
        SET(HAVE_WIN32_THREADS_H 1)
        ADD_DEFINITIONS(-DHAVE_WIN32_THREADS_H)
    ENDIF(CMAKE_USE_WIN32_THREADS_INIT)
ENDIF(CMAKE_USE_PTHREADS_INIT)

# interaction with Qt5 ---------------------------
IF(TRY_QT_LIB)
    FIND_PACKAGE(Qt5Core QUIET)
    IF(Qt5Core_FOUND)
        SET(HAVE_QT5 1)
        ADD_DEFINITIONS(-DHAVE_QT5)
        MESSAGE(STATUS "Qt5Core version: ${Qt5Core_VERSION}")
    ELSE(Qt5Core_FOUND)
        MESSAGE(STATUS "Qt5Core: not found")
    ENDIF(Qt5Core_FOUND)
ENDIF(TRY_QT_LIB)

SET(SYSTEM_LIBS ${QT_LIBRARIES}
                ${CMAKE_THREAD_LIBS_INIT}
                )

# other system libraries -------------------------
IF(WIN32)
    # needed for UUID
    SET(SYSTEM_LIBS Rpcrt4
            ${SYSTEM_LIBS}
            )
ENDIF(WIN32)

IF(UNIX)
    # on UNIXES add dl library
    SET(SYSTEM_LIBS dl
            ${SYSTEM_LIBS}
            )
ENDIF(UNIX)

# policies ---------------------------------------
IF(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
    cmake_policy(SET CMP0020 NEW)
    cmake_policy(SET CMP0043 NEW)
ENDIF(COMMAND cmake_policy)

# ==============================================================================
# output directories -----------------------------------------------------------
# ==============================================================================

SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/lib)

# ==============================================================================
# compiler setups --------------------------------------------------------------
# ==============================================================================

IF(NOT DEFINED COMPOSITE_PROJECT)
    SET(CMAKE_CXX_FLAGS_RELEASE "-O2")
    SET(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Werror -pedantic-errors -Wundef -Wno-long-long")
    #SET(CMAKE_BUILD_TYPE Release)
    SET(CMAKE_BUILD_TYPE Debug)
ENDIF(NOT DEFINED COMPOSITE_PROJECT)

# ==============================================================================
# project subdirectories  ------------------------------------------------------
# ==============================================================================

ADD_SUBDIRECTORY(src)
