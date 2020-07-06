add_library(smparser MsdFile.cpp EnumHelper.cpp RageTypes.cpp RageMath.cpp RageUtil.cpp)
target_include_directories(smparser PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)