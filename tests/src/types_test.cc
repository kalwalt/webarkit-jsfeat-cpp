#include <gtest/gtest.h>
#include <jsfeat.h>

TEST(TypesTests, TypesColors) {
    EXPECT_EQ(jsfeat::Colors::COLOR_RGBA2GRAY, 0);
}