

#include "jams_local.h"
#include "yaml-cpp/yaml.h"
#include <fstream>
#include <sstream>

bool JAMS_LoadConfig(char *filename)
{
	std::ifstream fin(filename);
	if (!fin.is_open()) {
		Error("Unable to open configuration file '%s' for reading", filename);
	}
	
	YAML::Parser parser(fin);

	YAML::Node config;
	if (parser.GetNextDocument(config) == false) {
		// Config empty
		Error("Configuration file '%s' is empty", filename);
		return false;
	}

	// Extract all config options from the file
	for (const option_t *opt = &jamsOptions[0]; opt->name != NULL; opt++)
	{
		assert(opt->ptr);
		assert(opt->defaultValue);
		switch (opt->type)
		{
			case OPT_BOOL:
				if (const YAML::Node *outBool = config.FindValue(opt->name)) {
					*outBool >> *(bool *)opt->ptr;
				} else {
					if ( !Strcmp(opt->defaultValue, "true") ) {
						*(bool *)opt->ptr = true;
					} else if ( !Strcmp(opt->defaultValue, "false") ) {
						*(bool *)opt->ptr = false;
					} else {
						Error("DERPDERPDERP");
					}
				}
				break;
			case OPT_INT:
				if (const YAML::Node *outInt = config.FindValue(opt->name)) {
					*outInt >> *(int *)opt->ptr;
				} else {
					int i;
					std::stringstream ss(opt->defaultValue);
					ss >> i;
					if ( ss.fail() ) {
						Error("DERPDERPDERP");
					}
					*(int *)opt->ptr = i;
				}
				break;
			case OPT_FLOAT:
				if (const YAML::Node *outFloat = config.FindValue(opt->name)) {
					*outFloat >> *(float *)opt->ptr;
				} else {
					float f;
					std::stringstream ss(opt->defaultValue);
					ss >> f;
					if ( ss.fail() ) {
						Error("DERPDERPDERP");
					}
					*(float *)opt->ptr = f;
				}
				break;
			case OPT_STRING:
				if (const YAML::Node *outString = config.FindValue(opt->name)) {
					*outString >> *(std::string *)opt->ptr;
				} else {
					*(std::string *)opt->ptr = std::string(opt->defaultValue);
				}
				break;
			default:
				Error("DERPDERPDERP");
				break;
		}
	}
}

bool JAMS_SaveConfig(char *filename)
{
	std::ofstream fout(filename);
	if (!fout.is_open()) {
		Error("Unable to open configuration file '%s' for writing", filename);
	}
}