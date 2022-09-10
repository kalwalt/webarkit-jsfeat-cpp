cmake_minimum_required( VERSION 3.1 )
project( JSFEAT_CPP )

set( CMAKE_CXX_STANDARD 14 )

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

set( CMAKE_CXX_FLAGS_RELEASE "-O0" )

set(EXPORTED_FUNCTIONS " -sEXPORTED_FUNCTIONS=_Grayscale,_Grayscale_m ")
set(EXPORTED_RUNTIME_METHODS " -sEXPORTED_RUNTIME_METHODS=cwrap ")
set(COMPRESSION_DB_FLAG " -O0 ")
set(DEBUG_FLAGS " -g -sASSERTIONS=1 --profiling -s DEMANGLE_SUPPORT=1 ")
set(MEMORY_OPTION " -s TOTAL_MEMORY=268435456 -s ALLOW_MEMORY_GROWTH=1 ")
set(POST_JS " --post-js ../js/post_bindings.api.js ")
set(ES6_BUILD " -s EXPORT_ES6=1 -s USE_ES6_IMPORT_META=0 -sEXPORT_NAME=jsfeatCpp -s MODULARIZE=1 ")
set(SINGLE_FILE " -s SINGLE_FILE=1")

include_directories(src)

add_executable(jsfeatcpp_debug emscripten/webarkitJsfeat.cpp )

set_target_properties(jsfeatcpp_debug PROPERTIES LINK_FLAGS " ${EXPORTED_FUNCTIONS} ${EXPORTED_RUNTIME_METHODS} ${COMPRESSION_DB_FLAG} ${MEMORY_OPTION} ${DEBUG_FLAGS} ${POST_JS} --bind ${ES6_BUILD} ${SINGLE_FILE}  ")

