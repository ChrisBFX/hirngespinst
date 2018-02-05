#pragma once
#include <tuple>


namespace hg
{
	namespace detail
	{
		template<typename T, typename Attributes = std::tuple<>, typename Children = std::tuple<>>
		struct iml
		{
			using type = T;

			Attributes attributes;

			Children children;

			template<typename  ... Attr, typename ... C>
			iml(std::tuple<Attr ...> attributes = std::tuple<>{}, std::tuple<C ...> children = std::tuple<>{}) :
				attributes(std::move(attributes)),
				children(std::move(children))
			{
			}

			template<typename ... Child>
			iml<T, Attributes, std::tuple<Child...>> operator()(Child&&... children) const
			{
				return iml<T, Attributes, std::tuple<Child...>>(std::move(attributes), std::make_tuple(std::move(children)...));
			}
		};
	}


	template<class T>
	auto node()
	{
		return detail::iml<T>{};
	}

	template<class T, class ...A>
	auto node(A&&... attributes)
	{
		return detail::iml<T, std::tuple<A...>>{std::make_tuple(std::move(attributes)...)};
	}
}
