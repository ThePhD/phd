// ficapi��
// The Shrug License

// Copyright � 2018 ThePhD. All rights can sometimes maybe not unreserved, yo.

#pragma once

#ifndef FICAPI_CXX_API_HPP
#define FICAPI_CXX_API_HPP

#include <ficapi/c_api.hpp>
#include <cstdint>

namespace ficapi {

	typedef ficapi_opaque opaque;
	typedef ficapi_opaque_handle opaque_handle;

	using type = ficapi_type;

	template <ficapi_type ft = ficapi_type::ficapi_type_int>
	struct deleter {
		void operator()(void* p) const {
			ficapi_delete(p, ft);
		}
	};

	struct int_deleter {
		FICAPI_API void operator()(int* p) const;
	};

	struct handle_deleter {
		FICAPI_API void operator()(opaque_handle p) const;
	};

	struct handle_no_alloc_deleter {
		FICAPI_API void operator()(opaque_handle p) const;
	};

	struct stateful_deleter {
	private:
		std::uint32_t m_some_state;
		ficapi_type m_ft;

	public:
		FICAPI_API stateful_deleter(std::uint32_t state, ficapi_type ft);

		FICAPI_API void operator()(void* p) const;

		FICAPI_API std::uint32_t state() const;

		FICAPI_API ficapi_type type() const;
	};

	struct stateful_int_deleter {
	private:
		std::uint32_t m_some_state;

	public:
		FICAPI_API stateful_int_deleter(std::uint32_t state);

		FICAPI_API void operator()(int* p) const;

		FICAPI_API std::uint32_t state() const;
	};

	struct stateful_handle_deleter {
	private:
		std::uint32_t m_some_state;

	public:
		FICAPI_API stateful_handle_deleter(std::uint32_t state);

		FICAPI_API void operator()(opaque_handle p) const;

		FICAPI_API std::uint32_t state() const;
	};

	struct stateful_handle_no_alloc_deleter {
	private:
		std::uint32_t m_some_state;

	public:
		FICAPI_API stateful_handle_no_alloc_deleter(std::uint32_t state);

		FICAPI_API void operator()(opaque_handle p) const;

		FICAPI_API std::uint32_t state() const;
	};
} // namespace ficapi

#endif // FICAPI_CXX_API_HPP
