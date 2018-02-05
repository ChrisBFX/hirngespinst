#pragma once
#include <memory>

namespace hg
{

	template<typename Attribute, typename Value>
	struct attribute_binder
	{
		Attribute name;
		Value& value;

		template<class T>
		void set(std::shared_ptr<T> obj)
		{
			auto& v = value;
			((*obj).*name).add_listener([obj, &v](const auto& objValue)
			{
				v = objValue;
			});
		}
	};


	template<typename Attribute>
	struct bindable_attribute
	{
		Attribute name;

		template<typename Value>
		constexpr attribute_binder<Attribute, Value> operator>>(Value&& value) const
		{
			return attribute_binder<Attribute, Value>{std::move(name), value};
		}
	};


	template<typename Attribute>
	constexpr auto bind(Attribute &&v)
	{
		return bindable_attribute<Attribute>{std::move(v)};
	}
}
