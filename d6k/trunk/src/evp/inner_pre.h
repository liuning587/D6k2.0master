#pragma once

#include <assert.h>
#include <stdint.h>

#ifdef __cplusplus
#include <iostream>
#include <memory>
#include <functional>
#endif // end of define __cplusplus

//! Define Operation System.
#if ( defined(WIN32) || defined(WIN64) || defined(_WIN64) )
#   ifndef H_OS_WINDOWS
#       define H_OS_WINDOWS
#   endif
#   ifndef H_WINDOWS_API
#       define H_WINDOWS_API
#   endif
#endif

#ifdef H_OS_WINDOWS
#include <ws2tcpip.h>
#include <ws2def.h>
#else
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h> // for TCP_NODELAY
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

#ifdef H_OS_WINDOWS
#define evp_usleep(us) Sleep((us)/1000)
#define snprintf  _snprintf
#define thread_local __declspec(thread)
#endif

#ifdef H_OS_WINDOWS
#define evpp_socket_t intptr_t
#else
#define evpp_socket_t int
#endif

#define signal_number_t evpp_socket_t


struct event;
namespace Evp
{
	int EventAdd(struct event* ev, const struct timeval* timeout);
	int EventDel(struct event*);
	int GetActiveEventCount();

	std::string strerror(int e);
}
