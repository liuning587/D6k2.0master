#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(QTCLASSLIBRARY1_LIB)
#  define CPREDICTION_EXPORT Q_DECL_EXPORT
# else
#  define CPREDICTION_EXPORT Q_DECL_IMPORT
# endif
#else
# define CPREDICTION_EXPORT
#endif
