#include "PrefsManager.h"

PrefsManager*	PREFSMAN = nullptr;	// global and accessible from anywhere in our program

PrefsManager::PrefsManager():
    m_bQuirksMode			( "QuirksMode",		false )
{

}