#include "AssimpUtility.h"

bool Utility::ParseFileName(aiString& str)
{
	const char* findBackSlash = strrchr(str.C_Str(), '\\');
	const char* findSlash = strrchr(str.C_Str(), '/');
	const char* findDot = strrchr(str.C_Str(), '.');
	if (findBackSlash)
	{
		str.Set(findBackSlash + 1);
		return true;
	}
	else if (findSlash)
	{
		str.Set(findSlash + 1);
		return true;
	}
	else if (findDot)
	{
		return true;
	}
	else
		return false;
}
