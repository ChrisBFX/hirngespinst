#pragma once
#include <functional>
#include <list>



namespace hg
{

	template<typename Payload = void>
	struct event
	{
		using listener = std::function<void(const Payload&)>;

		event() = default;
		event(const event&) = delete;
		event& operator=(const event&) = delete;

		void trigger(const Payload& payload) const
		{
			for (auto& fun : listeners)
			{
				fun(payload);
			}
		}

		void add_listener(listener fun)
		{
			listeners.push_back(std::move(fun));
		}

		std::list<listener> listeners;
	};

	template<typename T>
	struct bindable
	{
		using event = event<T>;
		using listener = typename event::listener;

		event change;
		T value;

		bindable() = default;
		bindable(const bindable<T>&) = delete;
		bindable<T>& operator=(const bindable<T>&) = delete;
		bindable(bindable<T>&&) = delete;
		bindable<T>& operator=(bindable<T>&&) = delete;

		operator T() const
		{
			return value;
		}

		bindable<T>& operator=(const T& l)
		{
			value = l;
			change.trigger(value);
			return *this;
		}

		void add_listener(listener fun)
		{
			fun(value);
			this->change.add_listener(std::move(fun));
		}
	};
}