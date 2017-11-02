#pragma once

#if defined(__APPLE__)
#define H_OS_MACOSX
#endif

#ifdef _DEBUG
#ifndef H_DEBUG_MODE
#define H_DEBUG_MODE
#endif
#endif

#include "windows_port.h"


#ifdef H_OS_WINDOWS
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif
