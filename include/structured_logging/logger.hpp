#pragma once
#include "field.hpp"
#include <type_traits>

namespace structured_logging
{
	template<class backend>
	class logger
	{
	public:
		logger(backend& b) : _backend(b) {}
		logger(backend&& b) : _backend(std::move(b)) {}

		template<class ... Args>
		void write(Args && ... args)
		{
			_backend.begin_entry();
			_write(std::forward<Args>(args)...);
			_backend.end_entry();
		}

	private:
		backend _backend;

		// peel off first argument
		template<class T, class ... Args>
		void _write(T && t, Args && ... args)
		{
			// force types like <const char(&)[42]> to decay to pointers, because we cannot specialise to_field on array length
			if constexpr (std::is_array_v<std::remove_reference_t<T>>)
			{
				_backend.store(
					to_field<std::decay_t<T>>(t)
				);
			}
			else
			{
				_backend.store(
					to_field<T>(t)
				);
			}
			_write(std::forward<Args>(args)...);
		}

		// end recursion
		void _write() {}
	};
}
