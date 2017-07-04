#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(BREAKER_LIB)
#  define BREAKER_EXPORT Q_DECL_EXPORT
# else
#  define BREAKER_EXPORT Q_DECL_IMPORT
# endif
#else
# define BREAKER_EXPORT
#endif
