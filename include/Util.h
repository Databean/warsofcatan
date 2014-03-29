#ifndef UTIL_H
#define UTIL_H

#include <utility>
#include <sstream>

typedef std::pair<int, int> Coordinate;
typedef std::pair<float, float> ScreenCoordinate;

template<class T> 
T fromString(const std::string& s) {
	std::stringstream stream(s);
	T ret;
	stream >> ret;
	return ret;
}

template<class T>
std::string toString(T t) {
	std::stringstream stream;
	stream << t;
	return stream.str();
}

#endif