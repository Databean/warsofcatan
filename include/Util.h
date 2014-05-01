#ifndef UTIL_H
#define UTIL_H

#include <utility>
#include <sstream>
#include <memory>

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

/**
 * Construct a resource unique_ptr, given a constructor, destructor, and arugments.
 * Intended for use with C-style constructor and destructor functions that operate on pointers.
 * From http://ericscottbarr.com/blog/2014/04/c-plus-plus-14-and-sdl2-managing-resources/
 * @param c The creator/initialization function.
 * @param d The destructor function
 * @param args The arguments to give to the constructor.
 * @return A unique pointer to the constructed resource.
 */
template<typename Creator, typename Destructor, typename... Arguments>
auto make_resource(Creator c, Destructor d, Arguments&&... args)
-> std::unique_ptr<typename std::decay<decltype(*c(std::forward<Arguments>(args)...))>::type, void(*)(typename std::decay<decltype(*c(std::forward<Arguments>(args)...))>::type*)>
{
	auto r = c(std::forward<Arguments>(args)...);
	if (!r) { throw std::runtime_error {"Unable to create resource"}; }
	typedef typename std::decay<decltype(*r)>::type ResourceType;
	return std::unique_ptr<ResourceType, void(*)(ResourceType*)>(r, d);
}

#endif
