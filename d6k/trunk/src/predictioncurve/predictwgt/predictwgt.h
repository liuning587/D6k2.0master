#pragma once

#include "qtclasslibrary1_global.h"
class QWidget;

extern "C"
{
	CPREDICTION_EXPORT QWidget * GetMainWgt();
	CPREDICTION_EXPORT void DestroyWgt();
}