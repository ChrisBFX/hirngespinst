#pragma once
#include "element.h"
#include <js/bind.hpp>
#include "../core/core.h"

namespace hg
{

	struct input : element
	{
		input() : element("input")
		{}

		void init() override
		{
			auto e = shared_from_this();
			auto self = std::dynamic_pointer_cast<input>(e);
			html.set("oninput", js::bind(&input::on_change, self, std::placeholders::_1));
		}

		void on_change(emscripten::val e)
		{
			auto v = html["value"].as<std::string>();
			value = v;
		};

		bindable<std::string> value;
	};
}
