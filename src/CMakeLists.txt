add_library(smparser global.cpp Attack.cpp BackgroundUtil.cpp RageException.cpp RageUtil.cpp MsdFile.cpp EnumHelper.cpp RageTypes.cpp RageMath.cpp LightsManager.cpp GameManager.cpp)
target_link_libraries(smparser pcre)
target_link_libraries(smparser "lua-5.1")
message(STATUS ${PCRE_LIBRARY})

target_include_directories(smparser PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)