#ifndef UTIL_H
#define UTIL_H

#include <utility>
#include <sstream>

typedef std::pair<int, int> Coordinate;
typedef std::pair<float, float> ScreenCoordinate;


/**
 * Converts any instance that has istream& operator>>(istream&, T&) defined from a string to its type.
 * @param s The string to read the value from
 * @return The value parsed from the string.
 */
template<class T> 
T fromString(const std::string& s) {
	std::stringstream stream(s);
	T ret;
	stream >> ret;
	return ret;
}

/**
 * Converts any instance that has ostream& operator<<(ostream&, T&) defined from its type to a string.
 * @param t The instance to convert to a string
 * @return The string created from the value.
 */
template<class T>
std::string toString(T t) {
	std::stringstream stream;
	stream << t;
	return stream.str();
}

#endif
