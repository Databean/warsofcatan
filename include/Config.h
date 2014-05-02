#ifndef CONFIG_H
#define CONFIG_H

#include <map>
#include <string>
#include <utility>
#include <fstream>

#include "Util.h"

/**
 * A configuration value stored in a configuration file.
 * These can auto-convert to several common types that are used.
 */
class ConfigValue {
private:
	std::string value;
public:
	ConfigValue(const std::string& val);
	ConfigValue(const ConfigValue&) = default;
	ConfigValue(ConfigValue&&) = default;
	~ConfigValue() = default;
	ConfigValue& operator=(const ConfigValue&) = default;
	ConfigValue& operator=(ConfigValue&&) = default;
	
	operator float() const;
	operator std::string() const;
	operator Coordinate() const;
	operator ScreenCoordinate() const;
	operator std::pair<ScreenCoordinate, ScreenCoordinate>() const;
};

/**
 * Configuration storage. Reads in configuration files in the format:
 * 
 * #Comment
 * name=value
 * 
 * Particular configuration values can be accessed by name and automatically to convert to common types.
 */
class Config {
private:
	std::map<std::string, ConfigValue> values;
	
	void init(std::istream& in);
public:
	Config(std::istream& source);
	Config(std::fstream&& source);
	Config(const std::string& filename);
	Config(const Config&) = default;
	Config(Config&&) = default;
	~Config() = default;
	Config& operator=(const Config&) = default;
	Config& operator=(Config&&) = default;
	
	const ConfigValue& operator[](const std::string& name) const;
};

const Config& getConfigFile(const std::string& filename);
const Config& getGraphicsConfig();

#endif