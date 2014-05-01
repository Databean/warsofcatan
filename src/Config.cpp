#include "Config.h"

#include <sstream>
#include <stdexcept>

#include "Util.h"

using std::string;
using std::stringstream;
using std::runtime_error;
using std::istream;
using std::fstream;

/**
 * Initialize a ConfigValue with the raw string it represents.
 * @param value The raw value.
 */
ConfigValue::ConfigValue(const string& value) : value(value) {
	
}

/**
 * Convert the ConfigValue into a float.
 * @return The text of the ConfigValue, interpreted as a float.
 */
ConfigValue::operator float() const {
	return fromString<float>(value);
}

/**
 * Convert the ConfigValue into a string.
 * @return The text of the ConfigValue.
 */
ConfigValue::operator string() const {
	return value;
}


/**
 * Read a pair (S,T) in from a stream.
 * @throws runtime_error If the pair is not in the form (S,T).
 * @param stream The stream to read in from.
 * @param ret The pair to read into.
 */
template<class S, class T>
void readPair(std::stringstream& stream, std::pair<S, T>& ret) {
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
	c = stream.get();
	if(c != ')') {
		throw runtime_error("Attempt to read a coordinate out of a non-coordinate value");
	}
}

/**
 * Convert the ConfigValue into a game Coordinate.
 * @throws runtime_error If the value cannot be converted.
 * @return The text of the ConfigValue, interpreted as a game Coordinate.
 */
ConfigValue::operator Coordinate() const {
	stringstream stream(value);
	Coordinate ret;
	readPair(stream, ret);
	return ret;
}

/**
 * Convert the ConfigValue into a ScreenCoordinate.
 * @throws runtime_error If the value cannot be converted.
 * @return The text of the ConfigValue, interpreted as a ScreenCoordinate.
 */
ConfigValue::operator ScreenCoordinate() const {
	stringstream stream(value);
	ScreenCoordinate ret;
	readPair(stream, ret);
	return ret;
}

/**
 * Convert the ConfigValue into a pair of ScreenCoordinates.
 * @throws runtime_error If the value cannot be converted.
 * @return The text of the ConfigValue, interpreted as a pair of ScreenCoordinates.
 */
ConfigValue::operator std::pair<ScreenCoordinate, ScreenCoordinate>() const {
	stringstream stream(value);
	std::pair<ScreenCoordinate, ScreenCoordinate> ret;
	char c = stream.get();
	if(c != '(') {
		throw runtime_error("Attempt to read a coordinate out of a non-coordinate value");
	}
	readPair(stream, ret.first);
	c = stream.get();
	if(c != ',') {
		throw runtime_error("Attempt to read a coordinate out of a non-coordinate value");
	}
	readPair(stream, ret.second);
	return ret;
}

/**
 * Initialize a Config file, reading from an input stream.
 * @param source The stream to read config values from.
 */
Config::Config(istream& source) {
	init(source);
}

/**
 * Initialize a Config file, reading from a file stream.
 * @param source The file stream to read config values from.
 */
Config::Config(fstream&& source) {
	init(source);
}

/**
 * Initialize a Config file, reading from a given file.
 * @param filename The path to the file to read.
 */
Config::Config(const string& filename) {
	fstream source(filename, fstream::in);
	init(source);
}

/**
 * Read in configuration values from a stream.
 * @param source The place to read configuration values from.
 */
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

/**
 * Retrieve a configuration value.
 * @throws runtime_error If no configuration value with that name exists.
 * @param name The name to retrieve from the configuration file.
 * @return The configuration value stored at the given name.
 */
const ConfigValue& Config::operator[](const string& name) const {
	auto it = values.find(name);
	if(it == values.end()) {
		throw runtime_error("No such key in the config");
	} else {
		return it->second;
	}
}

/**
 * Load a configuration file at a given path.
 * These configuration files are cached, and only will be loaded once.
 * @throws runtime_error If the file cannot be loaded.
 * @param filename The path to the file to read in.
 * @return The configuration stored at the file.
 */
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

/**
 * Load the graphics configuration file.
 * @throws runtime_error If the graphics configuration file is missing.
 * @return The graphics configuration.
 */
const Config& getGraphicsConfig() {
	return getConfigFile("resources/graphics.conf");
}
