#include "AssimpUtility.h"

bool Utility::ParseFileName(aiString& str)
{
	const char* findBackShlash = strrchr(str.C_Str(), '\\');
	if (findBackShlash)
	{
		str.Set(findBackShlash + 1);
		return true;
	}
	else
		return false;
}
