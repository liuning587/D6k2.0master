#pragma once

#include <typeinfo>
#include <algorithm>

namespace Evp
{

	// A variant type that can hold any other type.
	//
	// Usage 1 :
	//
	//    Buffer* buf(new Buffer());
	//    Any any(buf);
	//    Buffer* b = any_cast<Buffer*>(any);
	//    assert(buf == b);
	//    delete buf;
	//
	//
	// Usage 2 :
	//
	//    std::shared_ptr<Buffer> buf(new Buffer());
	//    Any any(buf);
	//    std::shared_ptr<Buffer> b = any_cast<std::shared_ptr<Buffer>>(any);
	//    assert(buf.get() == b.get());
	//
	//
	// Usage 3 :
	//
	//    std::shared_ptr<Buffer> buf(new Buffer());
	//    Any any(buf);
	//    std::shared_ptr<Buffer> b = any.Get<std::shared_ptr<Buffer>>();
	//    assert(buf.get() == b.get());
	//
	class Any 
	{
	public:
		Any() : m_pContent(nullptr) 
		{
		}
		~Any()
		{
			delete m_pContent;
		}

		template<typename ValueType>
		explicit Any(const ValueType& value)
			: m_pContent(new Holder<ValueType>(value)) 
		{

		}

		Any(const Any& other)
			: m_pContent(other.m_pContent ? other.m_pContent->clone() : nullptr) 
		{

		}

	public:
		Any& swap(Any& rhs) 
		{
			std::swap(m_pContent, rhs.m_pContent);
			return*this;
		}

		template<typename ValueType>
		Any& operator=(const ValueType& rhs) 
		{
			Any(rhs).swap(*this);
			return*this;
		}

		Any& operator=(const Any& rhs) 
		{
			Any(rhs).swap(*this);
			return*this;
		}

		bool IsEmpty() const 
		{
			return !m_pContent;
		}

		const std::type_info& GetType() const 
		{
			return m_pContent ? m_pContent->GetType() : typeid(void);
		}

		template<typename ValueType>
		ValueType operator()() const 
		{
			return Get<ValueType>();
		}

		template<typename ValueType>
		ValueType Get() const
		{
			if (GetType() == typeid(ValueType)) 
			{
				return static_cast<Any::Holder<ValueType>*>(m_pContent)->m_held;
			}
			else
			{
				return ValueType();
			}
		}
	protected:
		class PlaceHolder 
		{
		public:
			virtual ~PlaceHolder() 
			{
			}
		public:
			virtual const std::type_info& GetType() const = 0;
			virtual PlaceHolder* clone() const = 0;
		};

		template<typename ValueType>
		class Holder : public PlaceHolder
		{
		public:
			Holder(const ValueType& value): m_held(value)
			{

			}

			virtual const std::type_info& GetType() const
			{
				return typeid(ValueType);
			}

			virtual PlaceHolder* clone() const
			{
				return new Holder(m_held);
			}

			ValueType m_held;
		};

	protected:
		PlaceHolder* m_pContent;
		template<typename ValueType>
		friend ValueType* any_cast(Any*);
	};

	template<typename ValueType>
	ValueType* any_cast(Any* any) 
	{
		if (any && any->GetType() == typeid(ValueType))
		{
			return &(static_cast<Any::Holder<ValueType>*>(any->m_pContent)->held_);
		}

		return nullptr;
	}

	template<typename ValueType>
	const ValueType* any_cast(const Any* any)
	{
		return any_cast<ValueType>(const_cast<Any*>(any));
	}

	template<typename ValueType>
	ValueType any_cast(const Any& any)
	{
		const ValueType* result = any_cast<ValueType>(&any);
		assert(result);

		if (!result)
		{
			return ValueType();
		}

		return *result;
	}
}//namespace evp

