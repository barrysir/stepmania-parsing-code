add_executable(driver driver.cpp)
target_include_directories(driver PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../src>)
target_link_libraries(driver smparser)