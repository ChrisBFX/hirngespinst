#pragma once
#include <memory>
#include <string>
#include <emscripten/val.h>
#include "../core/core.h"

namespace hg
{

	struct element : std::enable_shared_from_this<element>
	{
		element() = default;

		element(std::string tag)
		{
			auto document = emscripten::val::global("document");
			html = document.call<emscripten::val>("createElement", tag);
		}

		virtual ~element()
		{}

		virtual void init()
		{}
		
		virtual emscripten::val getHtml()
		{
			return html;
		}
		emscripten::val html = emscripten::val::null();
	};
	
	struct container : element
	{
		container(std::string tag) : element(std::move(tag))
		{}

		std::vector<std::shared_ptr<element>> children;

		virtual void addChild(std::shared_ptr<element> element)
		{
			children.push_back(element);
			html.call<void>("appendChild", element->getHtml());
		}
	};

	struct div : container
	{
		div() : container("div")
		{}
	};



	struct text : element
	{
		text() : element("span")
		{
			value.add_listener([this](const std::string& newValue)
			{
				html.set("textContent", newValue);
			});
		}

		bindable<std::string> value;

		void set_value(const std::string& v)
		{
			value = v;
		}
	};
}
