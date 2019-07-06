


#include <phd/handle.hpp>

#include <ficapi/ficapi.hpp>

#include <phd/out_ptr.hpp>

#include <assert.hpp>

#include <type_traits>

// A template that always evaluates to false anyhow
template <std::size_t I>
struct always_false_index : std::integral_constant<bool, I == 1 && I == 0> {};

namespace phd { namespace out_ptr {

	// this is the full out_ptr_t customization point
	// you can also specialize the inout_ptr_t for that template too
	template <typename T, typename D, typename Pointer, typename... Args>
	class inout_ptr_t<phd::handle<T, D>, Pointer, Args...> : boost::empty_value<std::tuple<Args...>> {
	private:
		using Smart		 = phd::handle<T, D>;
		using source_pointer = pointer_of_or_t<Smart, Pointer>;
		using ArgsTuple	 = std::tuple<Args...>;
		using Base		 = boost::empty_value<ArgsTuple>;

		Pointer* m_target_ptr;

	public:
		inout_ptr_t(Smart& s, Args... args) noexcept
		: Base(std::forward<Args>(args)...), m_target_ptr(static_cast<Pointer*>(static_cast<void*>(std::addressof(s.get())))) {
		}

		inout_ptr_t(inout_ptr_t&& right) noexcept
		: Base(std::move(right)), m_target_ptr(right.m_target_ptr) {
			right.m_target_ptr = nullptr;
		}
		inout_ptr_t& operator=(inout_ptr_t&& right) noexcept {
			Base::operator	=(std::move(right));
			this->m_target_ptr = right.m_target_ptr;
			return *this;
		}

		operator Pointer*() const noexcept {
			return const_cast<Pointer*>(this->m_target_ptr);
		}

		~inout_ptr_t() noexcept {
			reset(std::make_index_sequence<std::tuple_size<ArgsTuple>::value>());
		}

	private:
		void reset(std::index_sequence<>) {
			// internal pointer has already been written into,
			// we are fine
		}

		template <std::size_t I0, std::size_t... I>
		void reset(std::index_sequence<I0, I...>) {
			static_assert(always_false_index<I0>::value, "you cannot reset the deleter or pass more arguments for handle<T, Deleter>!: it only takes one argument!");
		}
	};
}} // namespace phd::out_ptr

int main() {
	phd::handle<int*, ficapi::int_deleter> p(nullptr);
	ficapi_re_create(boost::out_ptr::inout_ptr<void*>(p), ficapi_type::ficapi_type_int);
	int* rawp = static_cast<int*>(p.get());

	OUT_PTR_C_ASSERT(rawp != nullptr);
	OUT_PTR_C_ASSERT(*rawp == ficapi_get_dynamic_data());
}
