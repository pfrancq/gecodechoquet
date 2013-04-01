PROJECT(testchoquet)
INCLUDE_DIRECTORIES(. ${GECODE_INCLUDE_DIR})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})


ADD_EXECUTABLE(testtestchoquet testclustering.cpp testretrieval testchoquet.cpp)
TARGET_LINK_LIBRARIES(testtestchoquet rcore rmath gecodechoquet)