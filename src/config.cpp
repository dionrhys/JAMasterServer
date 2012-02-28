

#include "jams_local.h"
#include "yaml-cpp/yaml.h"
#include <fstream>
#include <sstream>

bool JAMS_LoadConfig(char *filename)
{
	std::ifstream fin(filename);
	YAML::Parser parser(fin);

	YAML::Node config;
	if (parser.GetNextDocument(config) == false) {
		// Config empty
		Error("Configuration file is emturtles");
		return false;
	}

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
					bool b;
					std::stringstream ss(opt->defaultValue);
					ss >> b;
					if ( ss.fail() ) {
						Error("DERPDERPDERP");
					}
					*(bool *)opt->ptr = b;
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
				break;
			case OPT_STRING:
				break;
		}
	}
}