PROJECT(gecodechoquet)

SET(gecodechoquet_INST_INCLUDES
    rchoquetpropagator.h
    roptimizechoquet.h
    rtestchoquet.h
)

SET(gecodechoquet_TARGET_SOURCES
    rchoquetpropagator.cpp
    roptimizechoquet.cpp
    rtestchoquet.cpp
)

INCLUDE_DIRECTORIES(. ${GECODE_INCLUDE_DIR} ${gecodechoquet_INCLUDES})


IF(WIN32)
    ADD_LIBRARY(gecodechoquet STATIC ${gecodechoquet_TARGET_SOURCES})
    SET_TARGET_PROPERTIES(gecodechoquet PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(gecodechoquet rmath rcore ${Gecode_LIBRARIES})
ELSE(WIN32)
    ADD_LIBRARY(gecodechoquet SHARED ${gecodechoquet_TARGET_SOURCES})
    #ADD_SOURCE_FILE(gecodechoquet gecodechoquet_INCLUDES)
    TARGET_LINK_LIBRARIES(gecodechoquet rcore rmath dl ${Gecode_LIBRARIES})
    SET_TARGET_PROPERTIES(gecodechoquet PROPERTIES LT_VERSION_CURRENT 1
                                               VERSION 1.0.0
                                               SOVERSION 1)
    INSTALL(TARGETS gecodechoquet LIBRARY DESTINATION lib)
ENDIF(WIN32)

IF(rcmake-verbose)
    PRINT_LIST_WITH_MESSAGE("Will install the following header files for gecodechoquet:" gecodechoquet_INST_INCLUDES)
ENDIF(rcmake-verbose)

INSTALL(FILES ${gecodechoquet_INST_INCLUDES} DESTINATION include/r/gecodechoquet)