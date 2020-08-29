#include "global.h"
#include "smparser_SimfileLoader.h"
#include "NotesLoader.h"
#include "NotesLoaderSSC.h"
#include "NotesLoaderSM.h"
#include "NotesLoaderDWI.h"
#include "NotesLoaderJson.h"
#include "NotesWriterSM.h"
#include "NotesWriterSSC.h"
#include "NotesWriterDWI.h"
#include "NotesWriterJson.h"
#include <unordered_map>
#include <algorithm>
#include <filesystem>

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
        {"json", JSON}
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

std::string SimfileLoader::CleanPath(const std::string &path, bool as_directory) {
    // convert to absolute path
    std::filesystem::path absolute = std::filesystem::absolute(std::filesystem::path(path));
    // replace backslashes with slashes? maybe there's a better way
    std::string slashed = absolute.generic_string();
    // append a trailing slash, if this path is supposed to be a directory
    if (as_directory && slashed.back() != '/') {
        slashed.push_back('/');
    }
    return slashed;
}

std::string SimfileLoader::GetFileLoadedFromDir(const std::string &_dirpath) {
    std::string dirpath = CleanPath(_dirpath, true);

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

	DWILoader::GetApplicableFiles( dirpath, list );
	if( !list.empty() )
		return list[0];
    
	// BMSLoader::GetApplicableFiles( sPath, list );
	// if( !list.empty() )
	// 	return list[0];

	// KSFLoader::GetApplicableFiles( sPath, list );
	// if( !list.empty() )
	// 	return list[0];

    NotesLoaderJson::GetApplicableFiles(dirpath, list);
    if (!list.empty())
        return list[0];
    
    return "";
}

bool SimfileLoader::LoadFromDir(const std::string &_filepath, Song &out, bool tidyupdata) {
    // clean filepath
    std::string filepath = CleanPath(_filepath, true);

    std::set<RString> blacklist;
    auto oldSongDir = out.GetSongDir();
    out.SetSongDir(filepath);
    bool success = NotesLoader::LoadFromDir(filepath, out, blacklist);
    if (!success) {
        out.SetSongDir(oldSongDir);
    }
    if (success && tidyupdata) {
        // must have from_cache=false (or !from_cache == true)
        out.TidyUpData(false, false);
    }
    return success;
}

bool SimfileLoader::Load(const std::string &_filepath, Song &out, FileType format, bool tidyupdata) {
    // clean filepath
    std::string filepath = CleanPath(_filepath, false);

    if (format == DEFAULT) {
        format = GetTypeFromFilename(filepath);
    }

    bool success = false;
    auto oldSongDir = out.GetSongDir();
    out.SetSongDir(CleanPath(std::filesystem::path(filepath).parent_path().generic_string(), true));

    switch (format)
    {
        case SSC:
            {
                SSCLoader loader;
                success = loader.LoadFromSimfile(filepath, out); // will not run TidyUpData
            }
            break;
        case SM:
            {
                SMLoader loader;
                success = loader.LoadFromSimfile(filepath, out); // will not run TidyUpData
            }
            break;
        case DWI:
            {
                // custom function
                success = DWILoader::LoadFromSimfile(filepath, out); // will not run TidyUpData
            }
            break;
        case JSON:
            {
                success = NotesLoaderJson::LoadFromJsonFile(filepath, out); // will not run TidyUpData
            }
            break;
        case SMA:
        case BMS:
        case KSF:
        case NONE:
            return false;
        default:
            return false;
    };

    // run song tidyupdata
    if (success && tidyupdata) {
        // must have from_cache=false (or !from_cache == true)
        out.TidyUpData(false, false);
    }
    if (!success) {
        out.SetSongDir(oldSongDir);
    }
    return success;
}

bool SimfileLoader::LoadMsd(const std::string &_filepath, MsdFile &out, bool unescape) {
    std::string filepath = CleanPath(_filepath, false);
    return out.ReadFile(filepath, unescape);
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

bool SimfileLoader::Save(const std::string &_filepath, Song &out, FileType format) {
    // clean filepath
    std::string filepath = CleanPath(_filepath, false);

    if (format == DEFAULT) {
        format = GetTypeFromFilename(filepath);
    }

    switch (format)
    {
        case SSC:
            return SaveToSSCFile(filepath, out);
        case SM:
            return SaveToSMFile(filepath, out);
        case DWI:
            return NotesWriterDWI::Write(filepath, out);
        case JSON:  // not implemented yet
            return NotesWriterJson::WriteSong(filepath, out, true);
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