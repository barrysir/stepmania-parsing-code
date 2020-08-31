# Turn this option off to disable using WAV files with the game. Note that it is
# recommended to keep this on.
option(WITH_WAV "Build with WAV Support." ON)

# Turn this option off to disable using MP3 files with the game.
option(WITH_MP3 "Build with MP3 Support." ON)

# Turn this option off to disable using OGG files with the game.
option(WITH_OGG "Build with OGG/Vorbis Support." ON)

# Turn this on to compile tomcrypt with no assembly data. This is a portable
# mode.
# NOTE: The code will (probably) not compile if you turn this off.
#       If ENDIAN_LITTLE or ENDIAN_BIG is defined, libtomcrypt also requires
#       ENDIAN_32BITWORD or ENDIAN_64BITWORD to be defined. Stepmania does not
#       define these; instead, it bypasses the check by defining LTC_NO_ASM
#       (libtomcrypt skips the check if that gets defined), but it will
#       only define LTC_NO_ASM when this option is on.
#
#       tl;dr You need to define ENDIAN_32BITWORD or ENDIAN_64BITWORD
#       when this is turned off, and Stepmania doesn't do that.
#       Maybe something to fix in the future.
option(WITH_PORTABLE_TOMCRYPT
       "Build with assembly/free tomcrypt, making it portable." ON)