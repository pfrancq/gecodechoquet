MACRO(DO_GECODECHOQUET_R_LIB_INCLUDE _libPath)
        INCLUDE_DIRECTORIES("${_libPath}/gecodechoquet")
        LINK_DIRECTORIES(${_libPath}/build/gecodechoquet ${_libPath}/build/lib)
ENDMACRO(DO_GECODECHOQUET_R_LIB_INCLUDE)
