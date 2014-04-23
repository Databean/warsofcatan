#include "UnitTest++.h"
#include "Config.h"

using std::string;
using std::stringstream;

TEST(readOneString) {
	stringstream file("variable=value");
	Config config(file);
	
	CHECK((string)config["variable"] == "value");
}

TEST(readTwoStrings) {
	stringstream file("var_a=val_a\nvar_b=val_b\n");
	Config config(file);
	
	CHECK((string)config["var_a"] == "val_a");
	CHECK((string)config["var_b"] == "val_b");
}

TEST(readIntAndString) {
	stringstream file("string=hello\nint=5\n");
	Config config(file);
	
	CHECK((string)config["string"] == "hello");
	CHECK((int)config["int"] == 5);
}

TEST(readFloat) {
	stringstream file("float=5.0\n");
	Config config(file);
	
	CHECK((float)config["float"] == 5.0f);
}

TEST(readCommentsEmptyLines) {
	stringstream file("#this is a string\nstring=hello\n#we just had a string\n\n\n\n\n");
	Config config(file);
	
	CHECK((string)config["string"] == "hello");
}

TEST(readInvalid) {
	stringstream file("variable=value");
	Config config(file);
	
	CHECK_THROW((string)config["string"] == "hello", std::runtime_error);
}

TEST(readCoordinate) {
	stringstream file("coord=(1,2)");
	Config config(file);
	
	CHECK(Coordinate({1, 2}) == (Coordinate)config["coord"]);
}

TEST(readScreenCoordinate) {
	stringstream file("coord=(1.5,2.5)");
	Config config(file);
	
	CHECK(ScreenCoordinate({1.5, 2.5}) == (ScreenCoordinate)config["coord"]);
}
