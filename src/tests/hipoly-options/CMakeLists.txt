# ==============================================================================
# HiPoLy CMake File
# ==============================================================================

# program objects --------------------------------------------------------------
SET(TEST_XML_SRC
        test_options.cpp
        options.cpp
        )

# final build ------------------------------------------------------------------
ADD_EXECUTABLE(hipoly-options ${TEST_XML_SRC})
ADD_DEPENDENCIES(hipoly-options hipoly_shared)
IF(Qt5Core_FOUND)
    TARGET_LINK_LIBRARIES(hipoly-options Qt5::Core hipoly)
ELSE(Qt5Core_FOUND)
    TARGET_LINK_LIBRARIES(hipoly-options hipoly)
ENDIF(Qt5Core_FOUND)
