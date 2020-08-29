#include "PrefsManager.h"

PrefsManager*	PREFSMAN = nullptr;	// global and accessible from anywhere in our program

PrefsManager::PrefsManager():
	m_bShowNativeLanguage		( "ShowNativeLanguage",		true ),
	m_bAutogenSteps			( "AutogenSteps",			false ),
    m_bQuirksMode			( "QuirksMode",		false )
{

}