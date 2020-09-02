list(APPEND SM_PARSER_CPP RageSurface.cpp RageSurface_Load.cpp RageSurface_Load_BMP.cpp RageSurface_Load_GIF.cpp RageSurface_Load_JPEG.cpp RageSurface_Load_PNG.cpp)
list(APPEND SM_PARSER_CPP SongUtil.cpp StepsUtil.cpp NotesWriterJson.cpp NotesLoaderJSON.cpp JsonUtil.cpp NotesWriterDWI.cpp RageUtil_CharConversions.cpp GameInput.cpp NotesLoaderDWI.cpp PrefsManager.cpp LyricsLoader.cpp NotesWriterSSC.cpp global.cpp ActorUtil.cpp NotesLoaderSSC.cpp NotesWriterSM.cpp RageFile.cpp RageFileBasic.cpp RageFileDriverStd.cpp NotesLoaderSM.cpp RadarValues.cpp GameConstantsAndTypes.cpp Song.cpp SongUtil.cpp RageFileManager.cpp NotesLoader.cpp Steps.cpp CryptManager.cpp Style.cpp NoteDataUtil.cpp GameState.cpp NoteData.cpp TimingData.cpp TimingSegments.cpp LocalizedString.cpp NoteTypes.cpp RageLog.cpp PlayerNumber.cpp Difficulty.cpp Attack.cpp BackgroundUtil.cpp RageException.cpp RageUtil.cpp MsdFile.cpp EnumHelper.cpp RageTypes.cpp RageMath.cpp LightsManager.cpp GameManager.cpp)
list(APPEND SM_PARSER_CPP RageSoundReader_FileReader.cpp RageSoundReader_WAV.cpp)
if(HAS_MP3)
    list(APPEND SM_PARSER_CPP RageSoundReader_MP3.cpp)
endif()
if(HAS_OGG)
    list(APPEND SM_PARSER_CPP RageSoundReader_Vorbisfile.cpp)
endif()
list(APPEND SM_PARSER_CPP smparser.cpp smparser_SimfileLoader.cpp)

add_library(smparser ${SM_PARSER_CPP})
target_link_libraries(smparser pcre)
target_link_libraries(smparser "lua-5.1")
target_link_libraries(smparser tomcrypt)

target_link_libraries(smparser jsoncpp)
target_include_directories(smparser PUBLIC ${SM_EXTERN_DIR}/jsoncpp/include)

if(HAS_MP3)
    target_link_libraries(smparser mad)
endif()

if(HAS_OGG)
    target_link_libraries(smparser vorbisfile)
    target_link_libraries(smparser vorbis)
    target_link_libraries(smparser ogg)
endif()

target_link_libraries(smparser jpeg)
target_include_directories(smparser PUBLIC ${SM_EXTERN_DIR}/libjpeg)

target_link_libraries(smparser zlib)
target_link_libraries(smparser png)

# For some reason WIN32 is defined when libjpeg is compiled,
# but it's not defined when compiling smparser
# this changes the definition of the "boolean" typedef,
# causes the jpeg structs to be different sizes between smparser and libjpeg,
# and the code to error when it runs
# HERE'S A HACK, I DON'T KNOW WHAT CAUSES THIS
if(MSVC OR MINGW)
    sm_add_compile_definition(smparser WIN32)
endif()

sm_add_compile_definition(smparser CMAKE_POWERED)

# Mysterious compiler flag to make it compile
# See the CMake option for the reasoning behind this
if(WITH_PORTABLE_TOMCRYPT)
    sm_add_compile_definition(smparser LTC_NO_ASM)
endif()

target_include_directories(smparser PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

configure_file("${SM_SRC_DIR}/config.in.hpp"
               "${SM_BUILD_SRC_DIR}/generated/config.hpp")
target_include_directories(smparser PUBLIC ${SM_BUILD_SRC_DIR}/generated)