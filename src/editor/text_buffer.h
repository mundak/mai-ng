#pragma once

#include <cstdint>
#include <string>

class text_buffer
{
public:
  std::string& content();
  const std::string& content() const;
  void set_content(const std::string& text);
  void clear();
  bool is_empty() const;
  int32_t size() const;
  bool is_modified() const;
  void set_modified(bool value);

private:
  std::string m_content;
  bool m_modified = false;
};
