

#include "jams_local.h"
#include "yaml-cpp/yaml.h"
#include <fstream>
#include <sstream>

bool JAMS_LoadConfig(char *filename)
{
	std::ifstream fin(filename);
	if (!fin.is_open()) {
		Error("JAMS_LoadConfig: Unable to open configuration file '%s' for reading", filename);
	}
	
	YAML::Parser parser(fin);

	YAML::Node config;
	if (parser.GetNextDocument(config) == false) {
		// Config empty
		Error("JAMS_LoadConfig: Configuration file '%s' is empty", filename);
		return false;
	}

	// Extract all config options from the file
	for (const option_t *opt = &jamsOptions[0]; opt->name != NULL; opt++)
	{
		assert(opt->ptr);
		assert(opt->defaultValue);

		const YAML::Node *out;

		switch (opt->type)
		{
			case OPT_BOOL:
				out = config.FindValue(opt->name);
				if (out && out->Type() == YAML::NodeType::Scalar) {
					*out >> *(bool *)opt->ptr;
				} else {
					if ( !Strcmp(opt->defaultValue, "true") ) {
						*(bool *)opt->ptr = true;
					} else if ( !Strcmp(opt->defaultValue, "false") ) {
						*(bool *)opt->ptr = false;
					} else {
						Error("JAMS_LoadConfig: Failed to parse default value for %s", opt->name);
					}
				}
				break;
			case OPT_INT:
				out = config.FindValue(opt->name);
				if (out && out->Type() == YAML::NodeType::Scalar) {
					*out >> *(int *)opt->ptr;
				} else {
					int i;
					std::stringstream ss(opt->defaultValue);
					ss >> i;
					if ( ss.fail() ) {
						Error("JAMS_LoadConfig: Failed to parse default value for %s", opt->name);
					}
					*(int *)opt->ptr = i;
				}
				break;
			case OPT_FLOAT:
				out = config.FindValue(opt->name);
				if (out && out->Type() == YAML::NodeType::Scalar) {
					*out >> *(float *)opt->ptr;
				} else {
					float f;
					std::stringstream ss(opt->defaultValue);
					ss >> f;
					if ( ss.fail() ) {
						Error("JAMS_LoadConfig: Failed to parse default value for %s", opt->name);
					}
					*(float *)opt->ptr = f;
				}
				break;
			case OPT_STRING:
				out = config.FindValue(opt->name);
				if (out && out->Type() == YAML::NodeType::Scalar) {
					*out >> *(std::string *)opt->ptr;
				} else {
					*(std::string *)opt->ptr = std::string(opt->defaultValue);
				}
				break;
			default:
				Error("JAMS_LoadConfig: Invalid field type for ", opt->name);
				break;
		}
	}

	return true;
}

bool JAMS_SaveConfig(char *filename)
{
	std::ofstream fout(filename);
	if (!fout.is_open()) {
		Error("Unable to open configuration file '%s' for writing", filename);
	}

	return false;
}