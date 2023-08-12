# ==============================================================================
# HiPoLy CMake File
# ==============================================================================

# objects in library -----------------------------------------------------------
SET(HIPOLY_SRC
    # hipoly core ----------------------
        HiPoLyMainHeader.cpp

    # linked list ----------------------
        list/BaseList.cpp
        list/BaseIterator.cpp
        list/SimpleNode.cpp
        list/SimpleList.cpp
        list/SimpleIterator.cpp
        list/Object.cpp
        list/TemplNode.cpp
        list/TemplList.cpp
        list/TemplIterator.cpp

    # error system ---------------------
        error/ErrorSystem.cpp

    # cmd line options -----------------
        option/SmallGetOpt.cpp
        option/SimpleOptions.cpp

    # string manipulation --------------
        string/SmallString.cpp
        string/Password.cpp
        string/FileName.cpp
        string/FormatSpec.cpp

    # file systems ---------------------
        file/FortranIO.cpp
        file/FileSystem.cpp
        file/MemBlockFile.cpp
        file/DynamicPackage.cpp
        file/DirectoryEnum.cpp
        file/Terminal.cpp
        file/SystemFS.cpp
        file/StdIOFile.cpp

    # compress system ------------------
        compress/SimpleCompress.cpp

    # threading ------------------------
        thread/SmartThread.cpp
        thread/SmartThreadCommon.cpp
        thread/SimpleMutex.cpp
        thread/SimpleMutexCommon.cpp
        thread/SimpleCond.cpp
        thread/SimpleCondCommon.cpp
        thread/Runnable.cpp
        thread/ThreadPool.cpp

    # colors ---------------------------
        color/SmallColor.cpp

    # event ----------------------------
        event/UUID.cpp
        event/IndexCounter.cpp
        event/EventIdentifier.cpp
        event/SummaryEvents.cpp
        event/EventManager.cpp
        event/EventReceivers.cpp

    # time manipulation ----------------
        time/SmallTime.cpp
        time/SmallDate.cpp
        time/SmallTimeAndDate.cpp

    # XML subsystem --------------------
        xml/XMLNode.cpp
        xml/XMLDeclaration.cpp
        xml/XMLComment.cpp
        xml/XMLAttribute.cpp
        xml/XMLElement.cpp
        xml/XMLText.cpp
        xml/XMLBinData.cpp
        xml/XMLUnknown.cpp
        xml/XMLDocument.cpp
        xml/XMLIterator.cpp
        xml/XMLBinary.cpp
        xml/XMLPrinter.cpp
        xml/XMLParser.cpp

    # streams --------------------------
        stream/VerboseStr.cpp
        stream/TerminalStr.cpp
        )

IF(UNIX)
   # update list of source codes for UNIX platform
   SET(HIPOLY_SRC ${HIPOLY_SRC} binreloc/prefix.c)
ENDIF(UNIX)

IF(CMAKE_USE_PTHREADS_INIT)
   SET(HIPOLY_SRC ${HIPOLY_SRC}
        thread/SimpleCondPThread.cpp
        thread/SimpleMutexPThread.cpp
        thread/SmartThreadPThread.cpp
        )
ENDIF(CMAKE_USE_PTHREADS_INIT)

IF(HAVE_UUID_H)
    ADD_DEFINITIONS(-DHAVE_UUID_H)
ENDIF(HAVE_UUID_H)

# create static library --------------------------------------------------------
IF(LIBS_STATIC)
    ADD_LIBRARY(hipoly_static STATIC ${HIPOLY_SRC})
    IF(Qt5Core_FOUND)
        TARGET_LINK_LIBRARIES(hipoly_static Qt5:Core)
    ENDIF(Qt5Core_FOUND)
    SET_TARGET_PROPERTIES(hipoly_static PROPERTIES
                            OUTPUT_NAME hipoly
                            CLEAN_DIRECT_OUTPUT 1)
ENDIF(LIBS_STATIC)

# create shared library --------------------------------------------------------
IF(LIBS_SHARED)
    ADD_DEFINITIONS(-DHIPOLY_BUILDING_DLL)
    ADD_LIBRARY(hipoly_shared SHARED ${HIPOLY_SRC})
    IF(Qt5Core_FOUND)
        TARGET_LINK_LIBRARIES(hipoly_shared Qt5::Core ${SYSTEM_LIBS})
    ELSE(Qt5Core_FOUND)
        TARGET_LINK_LIBRARIES(hipoly_shared ${SYSTEM_LIBS})
    ENDIF(Qt5Core_FOUND)

    SET_TARGET_PROPERTIES(hipoly_shared PROPERTIES
                            OUTPUT_NAME hipoly
                            CLEAN_DIRECT_OUTPUT 1
                            VERSION ${HIPOLY_VERSION})
    INSTALL(TARGETS hipoly_shared
            DESTINATION lib)
ENDIF(LIBS_SHARED)

# ------------------------------------------------------------------------------
