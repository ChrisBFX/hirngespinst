#pragma once
#include "element.h"
#include "../core/core.h"
#include <js/bind.hpp>

namespace hg
{
	struct button : container
	{
		event<int> click;

		button() : container("button")
		{
		}

		void init() override
		{
			auto e = shared_from_this();
			auto self = std::dynamic_pointer_cast<button>(e);
			html.set("onclick", js::bind(&button::on_click, self, std::placeholders::_1));
		}

		void on_click(emscripten::val event) const
		{
			click.trigger(1);
		}
	};
}
