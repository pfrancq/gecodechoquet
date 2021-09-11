#ok, this file contain specific project definition
# - dependency checks
# - file to compile
# - sub project,
# - ...

#normaly all def in CMakeLists.txt
PROJECT(gecodechoquet CXX)

R_LOAD_DEPENDENCY("R" "R_LIB" "r")

#LINK_DIRECTORIES(${EXTRA_LIB_DIRECTORY})

# Add GeCode
FIND_PACKAGE(Gecode REQUIRED)
INCLUDE_DIRECTORIES(${GECODE_INCLUDE_DIR}/gecode)

SET(SUB_PROJECT gecodechoquet)
ADD_SUBDIRECTORY(gecodechoquet)
