#include "app/window.h"

#include <GLFW/glfw3.h>
#include <gtest/gtest.h>

class window_test : public ::testing::Test
{
protected:
  mai::app::window win;

  void SetUp() override { ASSERT_TRUE(win.init(1280, 720, "Mai IDE")); }

  void TearDown() override { win.shutdown(); }
};

TEST_F(window_test, init_and_shutdown)
{
  SUCCEED();
}

TEST_F(window_test, default_title)
{
  const char* title = glfwGetWindowTitle(win.handle());
  EXPECT_STREQ(title, "Mai IDE");
}

TEST_F(window_test, should_close_initially_false)
{
  EXPECT_FALSE(win.should_close());
}
