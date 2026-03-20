#include "editor/text_buffer.h"

#include <gtest/gtest.h>

TEST(TextBuffer, DefaultEmpty)
{
  text_buffer buf;
  EXPECT_TRUE(buf.is_empty());
  EXPECT_EQ(buf.size(), 0);
  EXPECT_FALSE(buf.is_modified());
}

TEST(TextBuffer, SetContent)
{
  text_buffer buf;
  buf.set_content("hello");
  EXPECT_EQ(buf.content(), "hello");
  EXPECT_EQ(buf.size(), 5);
  EXPECT_TRUE(buf.is_modified());
}

TEST(TextBuffer, Clear)
{
  text_buffer buf;
  buf.set_content("x");
  buf.clear();
  EXPECT_TRUE(buf.is_empty());
  EXPECT_EQ(buf.size(), 0);
  EXPECT_TRUE(buf.is_modified());
}

TEST(TextBuffer, ModifiedFlag)
{
  text_buffer buf;
  EXPECT_FALSE(buf.is_modified());

  buf.set_content("data");
  EXPECT_TRUE(buf.is_modified());

  buf.set_modified(false);
  EXPECT_FALSE(buf.is_modified());

  buf.set_modified(true);
  EXPECT_TRUE(buf.is_modified());
}

TEST(TextBuffer, MutableAccess)
{
  text_buffer buf;
  buf.content() = "modified directly";
  EXPECT_EQ(buf.content(), "modified directly");
  EXPECT_EQ(buf.size(), 17);
  EXPECT_FALSE(buf.is_modified());
}
