#pragma once
#include <iostream>

#ifdef _DEBUG
#define Debug_printf(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define Debug_wprintf(fmt, ...) wprintf(fmt, ##__VA_ARGS__)
#else
/** DEBUG ���忡���� �۵��Ǵ� ��ũ�� �Դϴ�.*/
#define Debug_printf(fmt, ...)
#define Debug_wprintf(fmt, ...)
#endif

