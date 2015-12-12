# ==============================================================================
# HiPoLy CMake File
# ==============================================================================

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# program objects --------------------------------------------------------------
SET(TEST_XML_SRC
        test_xml.cpp
        )

# final build ------------------------------------------------------------------
ADD_EXECUTABLE(hipoly-xml ${TEST_XML_SRC})
ADD_DEPENDENCIES(hipoly-xml hipoly_shared)
IF(Qt5Core_FOUND)
    QT5_USE_MODULES(hipoly-xml Core)
ENDIF(Qt5Core_FOUND)
TARGET_LINK_LIBRARIES(hipoly-xml hipoly)
