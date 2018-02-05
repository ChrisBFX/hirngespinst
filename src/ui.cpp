#include "hg/hirngespinst.h"

#include <stdio.h>
#include <iostream>
#include <hg/elements/button.h>
#include <hg/elements/input.h>
#include <emscripten/em_asm.h>

using namespace hg;

namespace ui
{
	
	class MyComponent : public element
	{
	public:
		std::shared_ptr<element> root;

		std::shared_ptr<text> buttonText;
		bindable<std::string> inputValue;

		MyComponent()
		{
			builder b;
			auto layout = node<hg::div>()(
				node<input>(bind(&input::value) >> inputValue),
				node<button>()(
					node<text>(set(&text::set_value) = "test")
					),
				node<button>(on(&button::click) = [this](int i) { buttonText->value = "button clicked."; })(
					node<text>(id = buttonText, set(&text::value) = "zweiter test")
					),
				node<text>(set(&text::value) = inputValue)
				);

			root = b.build(layout);
		}

		emscripten::val getHtml() override	{
			return root->getHtml();
		}
	};

}

int main() {	
	auto component = std::make_shared<ui::MyComponent>();

	auto document = emscripten::val::global("document");
	auto body = document["body"];
	body.call<emscripten::val>("appendChild", component->getHtml());
	auto c = [component] (emscripten::val e) { EM_ASM(console.log("whhhh"));};
	document.set("onclick", js::bind(c, std::placeholders::_1));
	return 0;
}