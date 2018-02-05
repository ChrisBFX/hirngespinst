#pragma once
#include <memory>


namespace hg
{

	template<typename Attribute, typename Func>
	struct event_binder
	{
		Attribute name;
		Func func;

		template<class T>
		void set(std::shared_ptr<T> obj)
		{
			auto f = func;
			((*obj).*name).add_listener([obj, f](const auto& event_payload)
			{
				f(event_payload);
			});
		}
	};


	template<typename Attribute>
	struct event_attribute
	{
		Attribute name;

		template<typename Value>
		constexpr event_binder<Attribute, Value> operator=(Value&& value) const
		{
			return event_binder<Attribute, Value>{std::move(name), std::move(value)};
		}
	};


	template<typename Attribute>
	constexpr auto on(Attribute &&v)
	{
		return event_attribute<Attribute>{std::move(v)};
	}


}
