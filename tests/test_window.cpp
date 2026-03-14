#include "app/window.h"

#include <GLFW/glfw3.h>
#include <gtest/gtest.h>

class WindowTest : public ::testing::Test
{
protected:
  mai::app::Window window;

  void SetUp() override { ASSERT_TRUE(window.init(1280, 720, "Mai IDE")); }

  void TearDown() override { window.shutdown(); }
};

TEST_F(WindowTest, InitAndShutdown)
{
  // SetUp + TearDown succeed without crashing
  SUCCEED();
}

TEST_F(WindowTest, DefaultTitle)
{
  const char* title = glfwGetWindowTitle(window.handle());
  EXPECT_STREQ(title, "Mai IDE");
}

TEST_F(WindowTest, ShouldCloseInitiallyFalse)
{
  EXPECT_FALSE(window.should_close());
}
