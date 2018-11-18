#include "../include/structured_logging.hpp"

struct Foo
{
	double x;
	int i;
};

namespace structured_logging
{
	template<> field to_field<Foo&>(Foo& foo)
	{
		return field
		{
			"Foo",
			{
				to_field(foo.x).tag_as("x"),
				to_field(foo.i).tag_as("i")
			}
		};
	}
}

int main()
{
	structured_logging::logger<structured_logging::cout_json_backend> log(structured_logging::cout_json_backend{});

	auto foo = Foo{ 3.1415, 42 };
	log.write(42, 1.0, "Hallo Welt", foo);
}