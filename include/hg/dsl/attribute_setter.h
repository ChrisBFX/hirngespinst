#pragma once
#include <memory>
#include <type_traits>
#include "../core/core.h"


namespace hg
{
	template<typename Attribute, typename Value, class Enable = void>
	struct attribute_setter
	{
		Attribute name;
		Value value;

		template<class T>
		void set(std::shared_ptr<T> obj)
		{
			((*obj).*name) = value;
		}
	};

	template<typename Attribute, typename Value>
	struct attribute_setter<Attribute, Value, std::enable_if_t<std::is_member_function_pointer<Attribute>::value>>
	{
		Attribute name;
		Value value;

		template<class T>
		void set(std::shared_ptr<T> obj)
		{
			((*obj).*name)(value);
		}
	};

	template<typename Attribute, typename Value>
	struct attribute_setter<Attribute, bindable<Value>>
	{
		Attribute name;
		bindable<Value>& value;

		template<class T>
		void set(std::shared_ptr<T> obj)
		{
			value.add_listener([obj, name = this->name](const Value& value)
			{
				((*obj).*name) = value;
			});
		}
	};



	template<typename Attribute>
	struct attribute
	{
		Attribute name;

		template<typename Value>
		constexpr attribute_setter<Attribute, Value> operator=(Value&& value) const
		{
			return attribute_setter<Attribute, Value>{std::move(name), std::move(value)};
		}

		template<typename Value>
		constexpr attribute_setter<Attribute, bindable<Value>> operator=(bindable<Value>& value) const
		{
			return attribute_setter<Attribute, bindable<Value>>{std::move(name), value};
		}
	};


	template<typename Attribute>
	constexpr auto set(Attribute &&v)
	{
		return attribute<Attribute>{std::move(v)};
	}

}
