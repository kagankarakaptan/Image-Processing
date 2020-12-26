#pragma once

#include "MyForm.h"
#include <windows.h>

BYTE* LoadBMP(int%, int%, long%, LPCTSTR);

bool SaveBMP(BYTE*, int, int, long, LPCTSTR);

BYTE* ConvertBMPToIntensity(BYTE*, int, int);

BYTE* ConvertIntensityToBMP(BYTE*, int, int, long%);
