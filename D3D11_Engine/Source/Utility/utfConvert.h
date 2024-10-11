#pragma once
#include <stdexcept>
#include <string>

namespace utfConvert
{
    std::string wstring_to_utf8(const std::wstring& wstr);
    std::wstring utf8_to_wstring(const std::string& str);
};
