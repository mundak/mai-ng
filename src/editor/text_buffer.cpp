#include "editor/text_buffer.h"

std::string& text_buffer::content()
{
  return m_content;
}

const std::string& text_buffer::content() const
{
  return m_content;
}

void text_buffer::set_content(const std::string& text)
{
  m_content = text;
  m_modified = true;
}

void text_buffer::clear()
{
  m_content.clear();
  m_modified = true;
}

bool text_buffer::is_empty() const
{
  return m_content.empty();
}

int32_t text_buffer::size() const
{
  return static_cast<int32_t>(m_content.size());
}

bool text_buffer::is_modified() const
{
  return m_modified;
}

void text_buffer::set_modified(bool value)
{
  m_modified = value;
}
