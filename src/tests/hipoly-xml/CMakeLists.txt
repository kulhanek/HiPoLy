# ==============================================================================
# HiPoLy CMake File
# ==============================================================================

# program objects --------------------------------------------------------------
SET(TEST_XML_SRC
        test_xml.cpp
        )

# final build ------------------------------------------------------------------
ADD_EXECUTABLE(hipoly-xml ${TEST_XML_SRC})
ADD_DEPENDENCIES(hipoly-xml hipoly_shared)
IF(Qt5Core_FOUND)
    TARGET_LINK_LIBRARIES(hipoly-xml Qt5::Core hipoly)
ELSE(Qt5Core_FOUND)
    TARGET_LINK_LIBRARIES(hipoly-xml hipoly)
ENDIF(Qt5Core_FOUND)

