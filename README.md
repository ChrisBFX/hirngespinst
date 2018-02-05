# hirngespinst

A project to play around with emscripten.

Goal is a web-gui-framework written in C++ which makes it easy to create complex application.

* **declarative** makes it easier to read your code (if you're familiar with xml/html)
currently a strange domain-specific-language is used

* **composable** simply combine base elements to form complex interfaces

* **data binding** 

* **dependency injection** to make it easy to depend on models and services in your components

## Requirements

* emscpripten
* js-bind (https://github.com/daminetreg/js-bind)
* gsl (tested with https://github.com/viboes/GSL)

## Example

```C++

#include <hg/elements/button.h>
#include <hg/elements/input.h>

using nsmespace hg;

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
```