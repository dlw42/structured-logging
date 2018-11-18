#pragma once
#include "field.hpp"
#include <iostream>

namespace structured_logging
{
	using namespace std;

	class cout_json_backend
	{
	public:
		void begin_entry()
		{
			cout << indent << "{\n";
			indent += "  ";
		}
		void begin_entry(const string& tag)
		{
			cout << indent << "\"" << tag << "\":\n" << indent << "{\n";
			indent += "  ";
		}
		void end_entry()
		{
			indent.erase(indent.size()-2, std::string::npos);
			cout << indent << "}\n";
		}
		void store(field&& f)
		{
			if (std::holds_alternative<string>(f.value)) { store_key_value(f.tag, get<string>(f.value));	}
			if (std::holds_alternative<double>(f.value)) { store_key_value(f.tag, get<double>(f.value)); }
			if (std::holds_alternative<bool>(f.value)) { store_key_value(f.tag, get<bool>(f.value)); }
			if (std::holds_alternative<int>(f.value)) { store_key_value(f.tag, get<int>(f.value)); }

			if (std::holds_alternative<vector<field>>(f.value))
			{
				begin_entry(f.tag);
				for (auto& f : get<vector<field>>(f.value))
				{
					store(std::move(f));
				}
				end_entry();
			}
		}

	private:
		string indent;

		template <typename V>
		//void store_key_value(string_view tag, V&& value) <-- not working on msvc, because has no << for string_view
		void store_key_value(const string& tag, V&& value)
		{
			cout << indent << "\"" << tag << "\": " << value << ",\n";
		}

		template <size_t indent>
		void store_key_value(const string& tag, double value)
		{
			cout << indent << "\"" << tag << "\": " << value << ",\n";
		}

		template <size_t indent>
		void store_key_value(const string& tag, string& value)
		{
			cout << indent << "\"" << tag << "\": \"" << value << "\",\n";
		}

	};
}
