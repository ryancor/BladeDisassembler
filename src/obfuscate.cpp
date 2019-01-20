#include <iostream>
#include <array>
#include <string>
#include "../inc/obfuscate.h"

// Non-Template Version
class obfuscate_string
{
  using String = std::basic_string<char>;

public:
  obfuscate_string(String s)
  {
    m_Storage = s;

    for(auto& c:m_Storage)
    {
        c ^= SECRET_CHAR;
    }
  }

  String GetString(const char *text)
  {
    return m_Storage;
  }

private:
  String m_Storage;
};

// template version
template<typename Char>
static const Char SECRET = 0x01;

template<typename Char, typename std::basic_string<Char>::size_type Length>
struct obfuscated_string
{
  using String = std::basic_string<Char>;

  const std::array<const Char, Length> storage;

  operator String() const
  {
    String s{storage.data(), Length};
    for(auto& c: s)
    {
      c ^= SECRET<Char>;
    }
    return s;
  }
};

template<typename ctype, ctype...STR>
constexpr obfuscated_string<ctype, sizeof... (STR)> operator ""_hidden()
{
  return { { (STR ^ SECRET<ctype>)... } };
}

// Using templates
void fileNotExist()
{
  static const auto message = "File does not exist"_hidden;
  std::string plaintext = message;
  std::cout << plaintext << std::endl;
}

void fileEmpty()
{
  static const auto message = "File is empty"_hidden;
  std::string plaintext = message;
  std::cout << plaintext << std::endl;
}

void fileNotReadable()
{
  static const auto message = "Can't read bytes from file"_hidden;
  std::string plaintext = message;
  std::cout << plaintext << std::endl;
}

void fileSizeLarge(const char *size)
{
  static const auto message = "File size too big : "_hidden;
  std::string plaintext = message;
  std::cout << plaintext + size << std::endl;
}

void stringBanner()
{
  static const auto message = "===== Strings ======"_hidden;
  std::string plaintext = message;
  std::cout << plaintext << std::endl;
}

// Using non-template
void printEncryptedString(const char *text)
{
  obfuscate_string obStr(text);
  std::string plaintext = obStr.GetString(text); // retrieves encrypted string
  UNHIDE_STRING((char*)plaintext.c_str());
  std::cout << plaintext << std::endl;
  HIDE_STRING((char*)plaintext.c_str());
}
