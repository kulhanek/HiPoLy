# ==============================================================================
# HiPoLy CMake File
# ==============================================================================

# program objects --------------------------------------------------------------
SET(TEST_XML_SRC
        test_uuid.cpp
        )

# final build ------------------------------------------------------------------
ADD_EXECUTABLE(hipoly-uuidgen ${TEST_XML_SRC})
ADD_DEPENDENCIES(hipoly-uuidgen hipoly_shared)
IF(Qt5Core_FOUND)
    QT5_USE_MODULES(hipoly-uuidgen Core)
ENDIF(Qt5Core_FOUND)
TARGET_LINK_LIBRARIES(hipoly-uuidgen hipoly)
