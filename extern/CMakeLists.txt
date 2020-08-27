include(CMakeProject-lua.cmake)
include(CMakeProject-pcre.cmake)
include(CMakeProject-libtomcrypt.cmake)

if(HAS_MP3)
    include(CMakeProject-mad.cmake)
endif()