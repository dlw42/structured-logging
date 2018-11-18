#pragma once
#include <variant>
#include <vector>
#include <string>
#include <initializer_list>

namespace structured_logging
{
	using std::vector;
	using std::string;
	using std::string_view;
	using std::variant;
	using std::initializer_list;

	struct field
	{
		using variant_t = variant<
			vector<field>,
			string,
			double,
			bool,
			int
		>;

		template<class Value>
		field(string_view t, Value&& v) : tag(t), value(v)
		{
			static_assert(
				std::is_convertible_v<Value, variant_t>,
				"Argument 2 is not convertible into variant_t. Either convert explicitly or extend variant_t"
				);
		}

		/// This ctor allows extenders to elide explicit vector conversion when defining custom types
		field(string_view t, initializer_list<field> list) : field(t, vector<field>(list)) {}

		/// use (unique!) tags for recognising structures in log
		string tag;

		/// tree node if vector, leaf if one of the non-vectors
		variant_t value;

		field& tag_as(string_view new_tag)
		{
			tag = new_tag;
			return *this;
		}
	};

	template <typename T> field to_field(T)
	{
		static_assert(false, "to_field(T) is missing template specialisation for this type");
	}

	template<> field to_field<const char*>(const char *s) { return field{ "string", string{s} }; }
	template<> field to_field<string_view>(string_view s) { return field{ "string", string{s} }; }
	template<> field to_field<double>(double d) { return field{ "double", d }; }
	template<> field to_field<bool>(bool b) { return field{ "bool", b }; }
	template<> field to_field<int>(int i) { return field{ "int", i }; }
}
