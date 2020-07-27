#include "global.h"
#include "smparser_SimfileLoader.h"
#include "NotesLoader.h"
#include "NotesLoaderSSC.h"
#include "NotesLoaderSM.h"
#include "NotesWriterSM.h"
#include "NotesWriterSSC.h"
#include <unordered_map>
#include <algorithm>

std::string GetExtension( const std::string &sPath )
{
	size_t pos = sPath.rfind( '.' );
	if( pos == sPath.npos )
		return std::string();

	size_t slash = sPath.find( '/', pos );
	if( slash != sPath.npos )
		return std::string(); /* rare: path/dir.ext/fn */

	return sPath.substr( pos+1, sPath.size()-pos+1 );
}

static inline void MakeLower(std::string &data) {
    std::transform(data.begin(), data.end(), data.begin(),
    [](unsigned char c){ return std::tolower(c); });
}

SimfileLoader::FileType SimfileLoader::GetTypeFromFilename(const std::string &filepath) {
    static std::unordered_map<std::string, FileType> EXTENSION_MAP{
        {"ssc", SSC},
        {"sm", SM},
        {"sma", SMA},
        {"dwi", DWI},
        {"ksf", KSF},
        {"bms", BMS},
        {"bml", BMS},
        {"bme", BMS},
        {"pms", BMS},
        // {"edit", ??},
        // {"ats", ??}
    };

    std::string extension = GetExtension(filepath);
    MakeLower(extension); // must do this because the code is expecting lowercase

    auto x = EXTENSION_MAP.find(extension);
    if (x == EXTENSION_MAP.end()) {
        return NONE;
    }
    return x->second;
}

std::string SimfileLoader::GetExtensionFromType(FileType type) {
    switch (type)
    {
        case SSC:   return "ssc";
        case SM:    return "sm";
        case DWI:   return "dwi";
        case JSON:  return "json";
        case SMA:   return "sma";
        case BMS:   return "bms";       // there are multiple valid filetypes, pick one of them to return
        case KSF:   return "ksf";
        case NONE:  return "";
        default:
            return "";
    };
}

std::string SimfileLoader::GetFileLoadedFromDir(const std::string &dirpath) {
    // This is copied from the NotesLoader code
	vector<RString> list;
    bool load_autosave = false;     // todo: make this a setting?

	SSCLoader loaderSSC;
	loaderSSC.GetApplicableFiles( dirpath, list, load_autosave );
	if( !list.empty() )
        return list[0];
    
	// SMALoader loaderSMA;
	// loaderSMA.GetApplicableFiles( sPath, list );
	// if (!list.empty() )
	// 	return list[0];
    
	SMLoader loaderSM;
	loaderSM.GetApplicableFiles( dirpath, list );
	if (!list.empty() )
        return list[0];

	// DWILoader::GetApplicableFiles( sPath, list );
	// if( !list.empty() )
	// 	return list[0];
    
	// BMSLoader::GetApplicableFiles( sPath, list );
	// if( !list.empty() )
	// 	return list[0];

	// KSFLoader::GetApplicableFiles( sPath, list );
	// if( !list.empty() )
	// 	return list[0];    
    return "";
}

bool SimfileLoader::LoadFromDir(const std::string &filepath, Song &out) {
    static std::set<RString> blacklist;
    auto oldSongDir = out.GetSongDir();
    out.SetSongDir(filepath);
    bool success = NotesLoader::LoadFromDir(filepath, out, blacklist);
    if (!success) {
        out.SetSongDir(oldSongDir);
    }
    return success;
}

bool SimfileLoader::Load(const std::string &filepath, Song &out, FileType format) {
    if (format == DEFAULT) {
        format = GetTypeFromFilename(filepath);
    }

    switch (format)
    {
        case SSC:
            {
                SSCLoader loader;
                return loader.LoadFromSimfile(filepath, out);
            }
        case SM:
            {
                SMLoader loader;
                return loader.LoadFromSimfile(filepath, out);
            }
        case SMA:
        case DWI:
        case BMS:
        case KSF:
        case JSON:
        case NONE:
            return false;
        default:
            return false;
    };
}

bool SimfileLoader::SaveToSSCFile(const RString &sPath, Song &out)
{
	vector<Steps*> vpStepsToSave;
	for (Steps *pSteps : out.GetSteps())
	{
		vpStepsToSave.push_back( pSteps );
	}
	for (Steps *s : out.GetUnknownStyleSteps())
	{
		vpStepsToSave.push_back(s);
	}

    // set bSavingCache to false
	return NotesWriterSSC::Write(sPath, out, vpStepsToSave, false);
}

bool SimfileLoader::SaveToSMFile(const RString &sPath, Song &out)
{
	vector<Steps*> vpStepsToSave;
	for (Steps *pSteps : out.GetSteps())
	{
		vpStepsToSave.push_back( pSteps );
	}
	for (Steps *s : out.GetUnknownStyleSteps())
	{
		vpStepsToSave.push_back(s);
	}

	return NotesWriterSM::Write( sPath, out, vpStepsToSave );
}

bool SimfileLoader::Save(const std::string &filepath, Song &out, FileType format) {
    if (format == DEFAULT) {
        format = GetTypeFromFilename(filepath);
    }

    switch (format)
    {
        case SSC:
            return SaveToSSCFile(filepath, out);
        case SM:
            return SaveToSMFile(filepath, out);
        case DWI:   // not implemented yet
        case JSON:  // not implemented yet
            return false;
        case SMA:   // not supported
            return false;
        case BMS:   // not supported
            return false;
        case KSF:   // not supported
            return false;
        case NONE:
            return false;
        default:
            return false;
    };
}
