#include "FileSystemUtility.h"

bool Utility::IsPathDirectory(const std::filesystem::path& path)
{
	// ��θ� ���ڿ��� ��ȯ�Ͽ� ������ ���ڸ� Ȯ��
	std::wstring pathStr = path.native();
	return (!pathStr.empty() && (pathStr.back() == L'/' || pathStr.back() == L'\\'));
}
