#pragma once
#include <memory>
#include "elements/element.h"

namespace hg
{

	struct builder
	{
		template<class T>
		struct n
		{
			template<class ...Child>
			n<T> operator()(Child&&... children)
			{
				return *this;
			}
		};

		template<class T, class ...A>
		n<T> e(A&&... attributes)
		{

		}

		template<size_t I = 0, class ...Child>
		inline typename std::enable_if<I == sizeof...(Child), void>::type
			build_children(container* parent, std::tuple<Child...>& children)
		{
		}

		template<size_t I = 0, class ...Child>
		inline typename std::enable_if<I < sizeof...(Child), void>::type
			build_children(container* parent, std::tuple<Child...>& children)
		{
			auto c = build(std::get<I>(children));
			parent->addChild(c);
			build_children<I + 1>(parent, children);
		}

		template<class Element, size_t I = 0, class ...Attribute>
		inline typename std::enable_if<I == sizeof...(Attribute), void>::type
			set_attributes(std::shared_ptr<Element> e, std::tuple<Attribute...>& attributes)
		{
		}

		template<class Element, size_t I = 0, class ...Attribute>
		inline typename std::enable_if<I < sizeof...(Attribute), void>::type
			set_attributes(std::shared_ptr<Element> e, std::tuple<Attribute...>& attributes)
		{
			auto& a = std::get<I>(attributes);
			a.set(e);
			set_attributes<Element, I + 1>(e, attributes);
		}

		template<class N, typename std::enable_if_t<std::is_base_of<container, typename N::type>::value, int> = 0>
		std::shared_ptr<element> build(N& n)
		{
			auto e = std::make_shared<typename N::type>();
			build_children(e.get(), n.children);
			set_attributes(e, n.attributes);
			e->init();
			return e;
		}

		template<class N, typename std::enable_if_t<!std::is_base_of<container, typename N::type>::value, int> = 0>
		std::shared_ptr<element> build(N& n)
		{
			auto e = std::make_shared<typename N::type>();
			set_attributes(e, n.attributes);
			e->init();
			return e;
		}
	};
}