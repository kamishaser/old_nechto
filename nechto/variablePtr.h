#pragma once
#include "nodePtr.h"

namespace nechto
{
	class variablePtr : public existing<nodePtr>
	{
	public:
		variablePtr(const existing<nodePtr>& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		variablePtr(const nodePtr& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return eptr.type() == nodeT::Variable;
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}
		void operator= (i64 value);
		void operator= (f64 value);

		operator i64() const;
		operator f64() const;
	};
	class i64VariablePtr : public variablePtr
	{
	public:
		i64VariablePtr(const variablePtr& eptr)
			:variablePtr(eptr)
		{
			assert(match(eptr));
		}
		i64VariablePtr(const existing<nodePtr>& eptr)
			:variablePtr(eptr)
		{
			assert(match(eptr));
		}
		i64VariablePtr(const nodePtr& eptr)
			:variablePtr(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const variablePtr& eptr)
		{
			return eptr.subtype();
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return variablePtr::match(eptr) && match(variablePtr(eptr));
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}
		i64 operator= (i64 value)
		{
			setData<i64>(value);
			return value;
		}
		i64 get() const
		{
			return getData<i64>();
		}
		void set(i64 value)
		{
			setData<i64>(value);
		}
		operator i64() const
		{
			return getData<i64>();
		}
		operator bool() const
		{
			return getData<i64>();
		}
	};
	class f64VariablePtr : public variablePtr
	{
	public:
		f64VariablePtr(const variablePtr& eptr)
			:variablePtr(eptr)
		{
			assert(match(eptr));
		}
		f64VariablePtr(existing<nodePtr>& eptr)
			:variablePtr(eptr)
		{
			assert(match(eptr));
		}
		f64VariablePtr(nodePtr& eptr)
			:variablePtr(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const variablePtr& eptr)
		{
			return !eptr.subtype();
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return variablePtr::match(eptr) && match(variablePtr(eptr));
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}
		f64 operator= (f64 value)
		{
			setData<f64>(value);
			return value;
		}
		f64 get() const
		{
			return getData<f64>();
		}
		void set(f64 value)
		{
			setData<f64>(value);
		}
		operator f64() const
		{
			return getData<f64>();
		}
	};

	void variablePtr::operator= (i64 value)
	{
		if (subtype())
			setData<i64>(value);
		else
			setData<f64>(static_cast<f64>(value));
	}
	void variablePtr::operator= (f64 value)
	{
		if (subtype())
			setData<i64>(static_cast<i64>(value));
		else
			setData<f64>(value);
	}

	variablePtr::operator i64() const
	{
		if (subtype())
			return getData<i64>();
		else
			return static_cast<i64>(getData<f64>());
	}
	variablePtr::operator f64() const
	{
		if (subtype())
			return static_cast<f64>(getData<i64>());
		else
			return getData<f64>();
	}
}