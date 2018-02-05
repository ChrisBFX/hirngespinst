#pragma once
#include <memory>

namespace hg
{

	struct id_helper
	{
		template<class T>
		struct id_setter
		{
			std::shared_ptr<T>& value;
			void set(std::shared_ptr<T> obj) const
			{
				value = obj;
			}
		};

		template<class T>
		constexpr id_setter<T> operator=(std::shared_ptr<T>& v) const
		{
			return id_setter<T>{ v };
		}
	};

	constexpr static id_helper id{};
}
