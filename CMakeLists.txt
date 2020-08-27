cmake_minimum_required(VERSION 3.15)
project(smparser VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

include(CMakeMacros.cmake)
include(DefineOptions.cmake)

if(WITH_WAV)
  # TODO: Identify which headers to check for ensuring this will always work.
  set(HAS_WAV TRUE)
endif()

if(WITH_MP3)
  if(WIN32 OR MACOSX)
    set(HAS_MP3 TRUE)
  else()
    find_package(Mad)
    if(NOT LIBMAD_FOUND)
      message(
        FATAL_ERROR
          "Libmad library not found. If you wish to skip mp3 support, set WITH_MP3 to OFF when configuring."
        )
    else()
      set(HAS_MP3 TRUE)
    endif()
  endif()
endif(WITH_MP3)

if(WITH_OGG)
  if(WIN32 OR MACOSX)
    set(HAS_OGG TRUE)
  else()
    find_package(Ogg)
    find_package(Vorbis)
    find_package(VorbisFile)

    if(NOT (OGG_FOUND AND VORBIS_FOUND AND VORBISFILE_FOUND))
      message(
        FATAL_ERROR
          "Not all vorbis libraries were found. If you wish to skip vorbis support, set WITH_OGG to OFF when configuring."
        )
    else()
      set(HAS_OGG TRUE)
    endif()
  endif()
endif()

add_subdirectory(extern)
add_subdirectory(src)
add_subdirectory(examples)