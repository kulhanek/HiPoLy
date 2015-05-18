# ==============================================================================
# HiPoLy CMake File
# ==============================================================================

# this is required for hipoly library ------------------------------------------
INCLUDE_DIRECTORIES(lib/hipoly SYSTEM)
INCLUDE_DIRECTORIES(lib/hipoly/compress SYSTEM)
INCLUDE_DIRECTORIES(lib/hipoly/error SYSTEM)
INCLUDE_DIRECTORIES(lib/hipoly/event SYSTEM)
INCLUDE_DIRECTORIES(lib/hipoly/file SYSTEM)
INCLUDE_DIRECTORIES(lib/hipoly/list SYSTEM)
INCLUDE_DIRECTORIES(lib/hipoly/option SYSTEM)
INCLUDE_DIRECTORIES(lib/hipoly/string SYSTEM)
INCLUDE_DIRECTORIES(lib/hipoly/thread SYSTEM)
INCLUDE_DIRECTORIES(lib/hipoly/time SYSTEM)
INCLUDE_DIRECTORIES(lib/hipoly/xml SYSTEM)
INCLUDE_DIRECTORIES(lib/hipoly/color SYSTEM)
INCLUDE_DIRECTORIES(lib/hipoly/stream SYSTEM)

# include subdirectories -------------------------------------------------------
ADD_SUBDIRECTORY(lib)
ADD_SUBDIRECTORY(tests)
