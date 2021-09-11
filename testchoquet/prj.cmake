#ok, this file contain specific project definition
# - dependency checks
# - file to compile
# - sub project,
# - ...

#normaly all def in CMakeLists.txt
PROJECT(testchoquet CXX)

R_LOAD_DEPENDENCY("R" "R_LIB" "r")
R_LOAD_DEPENDENCY("GECODECHOQUET" "GECODECHOQUET_LIB" "gecodechoquet")

SET(SUB_PROJECT testchoquet)
ADD_SUBDIRECTORY(src)
