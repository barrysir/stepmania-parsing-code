// This is in RageLog.cpp

#include "global.h"
#include <cstring>
#include <cstdio>
using namespace std;

void ShowWarningOrTrace( const char *file, int line, const char *message, bool bWarning )
{
	/* Ignore everything up to and including the first "src/". */
	const char *temp = strstr( file, "src/" );
	if( temp )
		file = temp + 4;

	// void (RageLog::*method)(const char *fmt, ...) = bWarning ? &RageLog::Warn : &RageLog::Trace;

	// if( LOG )
	// 	(LOG->*method)( "%s:%i: %s", file, line, message );
	// else
    fprintf( stderr, "%s:%i: %s\n", file, line, message );
}