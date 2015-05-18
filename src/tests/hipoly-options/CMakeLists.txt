# ==============================================================================
# HiPoLy CMake File
# ==============================================================================

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# program objects --------------------------------------------------------------
SET(TEST_XML_SRC
        test_options.cpp
        options.cpp
        )

# final build ------------------------------------------------------------------
ADD_EXECUTABLE(hipoly-options ${TEST_XML_SRC})
IF(Qt5Core_FOUND)
    QT5_USE_MODULES(hipoly-options Core)
ENDIF(Qt5Core_FOUND)
TARGET_LINK_LIBRARIES(hipoly-options hipoly)
