// ficapi™®
// The Shrug License

// Copyright © 2018 ThePhD. All rights can sometimes maybe not unreserved, yo.

#include <ficapi/c_api.hpp>
#include <ficapi/c++_api.hpp>

namespace ficapi {
	void int_deleter::operator()(int* p) const {
		ficapi_int_delete(p);
	}

	void handle_deleter::operator()(opaque_handle p) const {
		ficapi_handle_delete(p);
	}

	void handle_no_alloc_deleter::operator()(opaque_handle p) const {
		ficapi_handle_no_alloc_delete(p);
	}

	stateful_deleter::stateful_deleter(std::uint32_t state, ficapi_type ft)
	: m_some_state(state), m_ft(ft) {
	}

	void stateful_deleter::operator()(void* p) const {
		ficapi_delete(p, m_ft);
	}

	std::uint32_t stateful_deleter::state() const {
		return m_some_state;
	}

	ficapi_type stateful_deleter::type() const {
		return m_ft;
	}

	stateful_int_deleter::stateful_int_deleter(std::uint32_t state)
	: m_some_state(state) {
	}

	void stateful_int_deleter::operator()(int* p) const {
		ficapi_int_delete(p);
	}

	std::uint32_t stateful_int_deleter::state() const {
		return m_some_state;
	}

	stateful_handle_deleter::stateful_handle_deleter(std::uint32_t state)
	: m_some_state(state) {
	}

	void stateful_handle_deleter::operator()(opaque_handle p) const {
		ficapi_handle_delete(p);
	}

	std::uint32_t stateful_handle_deleter::state() const {
		return m_some_state;
	}

	stateful_handle_no_alloc_deleter::stateful_handle_no_alloc_deleter(std::uint32_t state)
	: m_some_state(state) {
	}

	void stateful_handle_no_alloc_deleter::operator()(opaque_handle p) const {
		ficapi_handle_no_alloc_delete(p);
	}

	std::uint32_t stateful_handle_no_alloc_deleter::state() const {
		return m_some_state;
	}
} // namespace ficapi
