#pragma once

#include "gettimeofday.h"

namespace Evp
{
	inline Timestamp::Timestamp()
		: m_nNanoSecond(0) 
	{
	}

	inline Timestamp::Timestamp(int64_t nanoseconds)
		: m_nNanoSecond(nanoseconds)
	{

	}

	inline bool Timestamp::IsEpoch() const 
	{
		return m_nNanoSecond == 0;
	}

	inline Timestamp::Timestamp(const struct timeval& t)
		: m_nNanoSecond(int64_t(t.tv_sec) * Duration::kSecond + t.tv_usec * Duration::kMicrosecond)
	{

	}

	inline Timestamp Timestamp::Now()
	{
		// gettimeofday have a higher performance than std::chrono::system_clock or std::chrono::high_resolution_clock
		// Detail benchmark can see benchmark/gettimeofday/gettimeofday.cc
#if 1
		return Timestamp(int64_t(utcmicrosecond() * Duration::kMicrosecond));
#else
		return Timestamp(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
#endif
	}

	inline void Timestamp::Add(Duration d)
	{
		m_nNanoSecond += d.Nanoseconds();
	}

	inline void Timestamp::To(struct timeval* t) const 
	{
		t->tv_sec = (long)(m_nNanoSecond / Duration::kSecond);
		t->tv_usec = (long)(m_nNanoSecond % Duration::kSecond) / (long)Duration::kMicrosecond;
	}

	inline struct timeval Timestamp::TimeVal() const
	{
		struct timeval t;
		To(&t);
		return t;
	}

	inline int64_t Timestamp::Unix() const 
	{
		return m_nNanoSecond / Duration::kSecond;
	}

	inline int64_t Timestamp::UnixNano() const
	{
		return m_nNanoSecond;
	}

	inline int64_t Timestamp::UnixMicro() const 
	{
		return m_nNanoSecond / Duration::kMicrosecond;
	}

	inline bool Timestamp::operator< (const Timestamp& rhs) const 
	{
		return m_nNanoSecond < rhs.m_nNanoSecond;
	}

	inline bool Timestamp::operator==(const Timestamp& rhs) const 
	{
		return m_nNanoSecond == rhs.m_nNanoSecond;
	}

	inline Timestamp Timestamp::operator+=(const Duration& rhs)
	{
		m_nNanoSecond += rhs.Nanoseconds();
		return *this;
	}

	inline Timestamp Timestamp::operator+(const Duration& rhs) const
	{
		Timestamp temp(*this);
		temp += rhs;
		return temp;
	}

	inline Timestamp Timestamp::operator-=(const Duration& rhs)
	{
		m_nNanoSecond -= rhs.Nanoseconds();
		return *this;
	}

	inline Timestamp Timestamp::operator-(const Duration& rhs) const 
	{
		Timestamp temp(*this);
		temp -= rhs;
		return temp;
	}

	inline Duration Timestamp::operator-(const Timestamp& rhs) const 
	{
		int64_t ns = m_nNanoSecond - rhs.m_nNanoSecond;
		return Duration(ns);
	}
} // namespace Evp

