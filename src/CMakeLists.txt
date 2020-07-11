add_library(smparser global.cpp GameState.cpp NoteData.cpp TimingData.cpp TimingSegments.cpp LocalizedString.cpp NoteTypes.cpp RageLog.cpp PlayerNumber.cpp Difficulty.cpp Attack.cpp BackgroundUtil.cpp RageException.cpp RageUtil.cpp MsdFile.cpp EnumHelper.cpp RageTypes.cpp RageMath.cpp LightsManager.cpp GameManager.cpp)
target_link_libraries(smparser pcre)
target_link_libraries(smparser "lua-5.1")
message(STATUS ${PCRE_LIBRARY})

target_include_directories(smparser PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)