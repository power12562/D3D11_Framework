#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>

namespace Utility
{
    //��ΰ� ���丮���� Ȯ���մϴ�
    bool IsPathDirectory(const std::filesystem::path& path);
}