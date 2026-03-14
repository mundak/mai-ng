#include <gtest/gtest.h>
#include <string>

TEST(SmokeTest, TrueIsTrue)
{
  EXPECT_TRUE(true);
}

TEST(SmokeTest, ProjectName)
{
  std::string name = "Mai IDE";
  EXPECT_EQ(name, "Mai IDE");
}
