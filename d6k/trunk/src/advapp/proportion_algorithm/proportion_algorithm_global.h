#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(PROPORTION_ALGORITHM_LIB)
#  define PROPORTION_ALGORITHM_EXPORT Q_DECL_EXPORT
# else
#  define PROPORTION_ALGORITHM_EXPORT Q_DECL_IMPORT
# endif
#else
# define PROPORTION_ALGORITHM_EXPORT
#endif

