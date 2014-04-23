#include "Config.h"

#include <sstream>
#include <stdexcept>

#include "Util.h"

using std::string;
using std::stringstream;
using std::runtime_error;
using std::istream;
using std::fstream;

ConfigValue::ConfigValue(const string& value) : value(value) {
	
}

ConfigValue::operator int() const {
	return fromString<int>(value);
}

ConfigValue::operator float() const {
	return fromString<float>(value);
}

ConfigValue::operator string() const {
	return value;
}

ConfigValue::operator Coordinate() const {
	stringstream stream(value);
	Coordinate ret;
	char c = stream.get();
	if(c != '(') {
		throw runtime_error("Attempt to read a coordinate out of a non-coordinate value");
	}
	stream >> ret.first;
	c = stream.get();
	if(c != ',') {
		throw runtime_error("Attempt to read a coordinate out of a non-coordinate value");
	}
	stream >> ret.second;
	return ret;
}

ConfigValue::operator ScreenCoordinate() const {
	stringstream stream(value);
	ScreenCoordinate ret;
	char c = stream.get();
	if(c != '(') {
		throw runtime_error("Attempt to read a coordinate out of a non-coordinate value");
	}
	stream >> ret.first;
	c = stream.get();
	if(c != ',') {
		throw runtime_error("Attempt to read a coordinate out of a non-coordinate value");
	}
	stream >> ret.second;
	return ret;
}

Config::Config(istream& source) {
	init(source);
}

Config::Config(fstream&& source) {
	init(source);
}

Config::Config(const string& filename) {
	fstream source(filename, fstream::in);
	init(source);
}

void Config::init(istream& source) {
	string line;
	while(source) {
		std::getline(source, line);
		if(line[0] == '#' || line.find('=') == string::npos) {
			continue;
		}
		string name = line.substr(0, line.find('='));
		string value = line.substr(line.find('=') + 1);
		values.insert(std::make_pair(name, ConfigValue(value)));
	}
}

const ConfigValue& Config::operator[](const string& name) const {
	auto it = values.find(name);
	if(it == values.end()) {
		throw runtime_error("No such key in the config");
	} else {
		return it->second;
	}
}

const Config& getConfigFile(const std::string& filename) {
	static std::map<std::string, Config> configs;
	
	auto it = configs.find(filename);
	if(it == configs.end()) {
		fstream file(filename);
		if(!file) {
			throw runtime_error("Cannot open file " + filename);
		}
		return configs.insert(std::make_pair(filename, Config(file))).first->second;
	} else {
		return it->second;
	}
}

const Config& getGraphicsConfig() {
	return getConfigFile("resources/graphics.conf");
}
