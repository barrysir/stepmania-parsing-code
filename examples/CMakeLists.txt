list(APPEND EXECUTABLES dumpmsd dumpsim convert)

add_executable(dumpmsd dumpmsd.cpp)
add_executable(dumpsim dumpsim.cpp)
add_executable(convert convert.cpp)

foreach(target ${EXECUTABLES})
    target_include_directories(${target} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../src>)
    target_link_libraries(${target} smparser)    
endforeach(target ${EXECUTABLES})

# trying to reduce code size
# target_compile_options(dumpsim PUBLIC -ffunction-sections -fdata-sections)