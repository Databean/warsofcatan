#include <stdexcept>

#include "gtest/gtest.h"

#include "Config.h"


using std::string;
using std::stringstream;

TEST(ConfigTest, readOneString) {
	stringstream file("variable=value");
	Config config(file);
	
	ASSERT_EQ((string)config["variable"], "value");
}

TEST(ConfigTest, readTwoStrings) {
	stringstream file("var_a=val_a\nvar_b=val_b\n");
	Config config(file);
	
	ASSERT_EQ((string)config["var_a"], "val_a");
	ASSERT_EQ((string)config["var_b"], "val_b");
}

TEST(ConfigTest, readFloat) {
	stringstream file("float=5.0\n");
	Config config(file);
	
	ASSERT_EQ((float)config["float"], 5.0f);
}

TEST(ConfigTest, readCommentsEmptyLines) {
	stringstream file("#this is a string\nstring=hello\n#we just had a string\n\n\n\n\n");
	Config config(file);
	
	ASSERT_EQ((string)config["string"], "hello");
}

TEST(ConfigTest, readInvalid) {
	stringstream file("variable=value");
	Config config(file);
	
	ASSERT_THROW((string)config["string"] == "hello", std::runtime_error);
}

TEST(ConfigTest, readCoordinate) {
	stringstream file("coord=(1,2)");
	Config config(file);
	
	ASSERT_EQ(Coordinate({1, 2}), (Coordinate)config["coord"]);
}

TEST(ConfigTest, readScreenCoordinate) {
	stringstream file("coord=(1.5,2.5)");
	Config config(file);
	
	ASSERT_EQ(ScreenCoordinate({1.5, 2.5}), (ScreenCoordinate)config["coord"]);
}
