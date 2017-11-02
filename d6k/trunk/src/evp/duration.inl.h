#pragma once

namespace Evp
{
	inline Duration::Duration() : m_nNanoSeconds(0) 
	{

	}

	inline Duration::Duration(const struct timeval& t)
		: m_nNanoSeconds(t.tv_sec * kSecond + t.tv_usec * kMicrosecond) 
	{

	}

	inline Duration::Duration(int64_t nanoseconds)	: m_nNanoSeconds(nanoseconds) 
	{

	}

	inline Duration::Duration(int nanoseconds)	: m_nNanoSeconds(nanoseconds) 
	{

	}

	inline Duration::Duration(double seconds)
		: m_nNanoSeconds((int64_t)(seconds * kSecond)) 
	{

	}

	inline int64_t Duration::Nanoseconds() const 
	{
		return m_nNanoSeconds;
	}

	inline double Duration::Seconds() const 
	{
		return double(m_nNanoSeconds) / kSecond;
	}

	inline double Duration::Milliseconds() const 
	{
		return double(m_nNanoSeconds) / kMillisecond;
	}

	inline double Duration::Microseconds() const 
	{
		return double(m_nNanoSeconds) / kMicrosecond;
	}

	inline double Duration::Minutes() const 
	{
		return double(m_nNanoSeconds) / kMinute;
	}

	inline double Duration::Hours() const 
	{
		return double(m_nNanoSeconds) / kHour;
	}

	inline bool Duration::IsZero() const 
	{
		return m_nNanoSeconds == 0;
	}

	inline struct timeval Duration::TimeVal() const
	{
		struct timeval t;
		To(&t);
		return t;
	}

	inline void Duration::To(struct timeval* t) const 
	{
		t->tv_sec = (long)(m_nNanoSeconds / kSecond);
		t->tv_usec = (long)(m_nNanoSeconds % kSecond) / (long)kMicrosecond;
	}

	inline bool Duration::operator<(const Duration& rhs) const 
	{
		return m_nNanoSeconds < rhs.m_nNanoSeconds;
	}

	inline bool Duration::operator<=(const Duration& rhs) const 
	{
		return m_nNanoSeconds <= rhs.m_nNanoSeconds;
	}

	inline bool Duration::operator>(const Duration& rhs) const 
	{
		return m_nNanoSeconds > rhs.m_nNanoSeconds;
	}

	inline bool Duration::operator>=(const Duration& rhs) const 
	{
		return m_nNanoSeconds >= rhs.m_nNanoSeconds;
	}

	inline bool Duration::operator==(const Duration& rhs) const 
	{
		return m_nNanoSeconds == rhs.m_nNanoSeconds;
	}

	inline Duration Duration::operator+=(const Duration& rhs) 
	{
		m_nNanoSeconds += rhs.m_nNanoSeconds;
		return *this;
	}

	inline Duration Duration::operator-=(const Duration& rhs) 
	{
		m_nNanoSeconds -= rhs.m_nNanoSeconds;
		return *this;
	}

	inline Duration Duration::operator*=(int n) 
	{
		m_nNanoSeconds *= n;
		return *this;
	}

	inline Duration Duration::operator/=(int n) 
	{
		m_nNanoSeconds /= n;
		return *this;
	}
} // namespace Evp

