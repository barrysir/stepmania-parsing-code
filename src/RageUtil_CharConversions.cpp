#include "global.h"
#include "RageUtil_CharConversions.h"
#include "RageUtil.h"
#include "RageLog.h"
// barry edit - added missing include
#include "RageException.h"

#if defined(_WINDOWS)

// barry edit - commenting: only using this function
// #include "archutils/Win32/ErrorStrings.h"
RString werr_ssprintf( int err, const char *fmt, ... )
{
	char buf[1024] = "";
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		0, err, 0, buf, sizeof(buf), nullptr);

	// Why is FormatMessage returning text ending with \r\n? (who? -aj)
	// Perhaps it's because you're on Windows, where newlines are \r\n. -aj
	RString text = buf;
	text.Replace( "\n", "" );
	text.Replace( "\r", " " ); // foo\r\nbar -> foo bar
	TrimRight( text ); // "foo\r\n" -> "foo"

	va_list	va;
	va_start(va, fmt);
	RString s = vssprintf( fmt, va );
	va_end(va);

	return s += ssprintf( " (%s)", text.c_str() );
}

#include <windows.h>

/* Convert from the given codepage to UTF-8.  Return true if successful. */
static bool CodePageConvert( RString &sText, int iCodePage )
{
	int iSize = MultiByteToWideChar( iCodePage, MB_ERR_INVALID_CHARS, sText.data(), sText.size(), nullptr, 0 );
	if( iSize == 0 )
	{
		LOG->Trace( "%s\n", werr_ssprintf(GetLastError(), "err: ").c_str() );
		return false; /* error */
	}

	wstring sOut;
	sOut.append( iSize, ' ' );
	/* Nonportable: */
	iSize = MultiByteToWideChar( iCodePage, MB_ERR_INVALID_CHARS, sText.data(), sText.size(), (wchar_t *) sOut.data(), iSize );
	ASSERT( iSize != 0 );

	sText = WStringToRString( sOut );
	return true;
}

static bool AttemptEnglishConversion( RString &sText ) { return CodePageConvert( sText, 1252 ); }
static bool AttemptKoreanConversion( RString &sText ) { return CodePageConvert( sText, 949 ); }
static bool AttemptJapaneseConversion( RString &sText ) { return CodePageConvert( sText, 932 ); }

#elif defined(HAVE_ICONV)
#include <errno.h>
#include <iconv.h>

static bool ConvertFromCharset( RString &sText, const char *szCharset )
{
	iconv_t converter = iconv_open( "UTF-8", szCharset );
	if( converter == (iconv_t) -1 )
	{
		LOG->MapLog( ssprintf("conv %s", szCharset), "iconv_open(%s): %s", szCharset, strerror(errno) );
		return false;
	}

	/* Copy the string into a char* for iconv */
	ICONV_CONST char *szTextIn = const_cast<ICONV_CONST char*>( sText.data() );
	size_t iInLeft = sText.size();

	/* Create a new string with enough room for the new conversion */
	RString sBuf;
	sBuf.resize( sText.size() * 5 );

	char *sTextOut = const_cast<char*>( sBuf.data() );
	size_t iOutLeft = sBuf.size();
	size_t size = iconv( converter, &szTextIn, &iInLeft, &sTextOut, &iOutLeft );

	iconv_close( converter );

	if( size == (size_t)(-1) )
	{
		LOG->Trace( "%s\n", strerror( errno ) );
		return false; /* Returned an error */
	}

	if( iInLeft != 0 )
	{
		LOG->Warn( "iconv(UTF-8,%s) for \"%s\": whole buffer not converted (%i left)", szCharset, sText.c_str(), int(iInLeft) );
		return false;
	}

	if( sBuf.size() == iOutLeft )
		return false; /* Conversion failed */

	sBuf.resize( sBuf.size()-iOutLeft );

	sText = sBuf;
	return true;
}

static bool AttemptEnglishConversion( RString &sText ) { return ConvertFromCharset( sText, "CP1252" ); }
static bool AttemptKoreanConversion( RString &sText ) { return ConvertFromCharset( sText, "CP949" ); }
static bool AttemptJapaneseConversion( RString &sText ) { return ConvertFromCharset( sText, "CP932" ); }

#elif defined(MACOSX)
#include <CoreFoundation/CoreFoundation.h>

static bool ConvertFromCP( RString &sText, int iCodePage )
{
	CFStringEncoding encoding = CFStringConvertWindowsCodepageToEncoding( iCodePage );
	
	if( encoding == kCFStringEncodingInvalidId )
		return false;
	
	CFStringRef old = CFStringCreateWithCString( kCFAllocatorDefault, sText, encoding );
	
	if( old == nullptr )
		return false;
	const size_t size = CFStringGetMaximumSizeForEncoding( CFStringGetLength(old), kCFStringEncodingUTF8 );
	
	char *buf = new char[size+1];
	buf[0] = '\0';
	bool result = CFStringGetCString( old, buf, size, kCFStringEncodingUTF8 );
	sText = buf;
	delete[] buf;
	CFRelease( old );
	return result;
}

static bool AttemptEnglishConversion( RString &sText ) { return ConvertFromCP( sText, 1252 ); }
static bool AttemptKoreanConversion( RString &sText ) { return ConvertFromCP( sText, 949 ); }
static bool AttemptJapaneseConversion( RString &sText ) { return ConvertFromCP( sText, 932 ); }

#else

/* No converters are available, so all fail--we only accept UTF-8. */
static bool AttemptEnglishConversion( RString &sText ) { return false; }
static bool AttemptKoreanConversion( RString &sText ) { return false; }
static bool AttemptJapaneseConversion( RString &sText ) { return false; }

#endif

bool ConvertString( RString &str, const RString &encodings )
{
	if( str.empty() )
		return true;

	vector<RString> lst;
	split( encodings, ",", lst );

	for(unsigned i = 0; i < lst.size(); ++i)
	{
		if( lst[i] == "utf-8" )
		{
			/* Is the string already valid utf-8? */
			if( utf8_is_valid(str) )
				return true;
			continue;
		}
		if( lst[i] == "english" )
		{
			if( AttemptEnglishConversion(str) )
				return true;
			continue;
		}

		if( lst[i] == "japanese" )
		{
			if( AttemptJapaneseConversion(str) )
				return true;
			continue;
		}

		if( lst[i] == "korean" )
		{
			if( AttemptKoreanConversion(str) )
				return true;
			continue;
		}

		RageException::Throw( "Unexpected conversion string \"%s\" (string \"%s\").",
				      lst[i].c_str(), str.c_str() );
	}

	return false;
}

/* Written by Glenn Maynard.  In the public domain; there are so many
 * simple conversion interfaces that restricting them is silly. */
