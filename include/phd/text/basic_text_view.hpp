#pragma once
#ifndef PHD_TEXT_BASIC_TEXT_VIEW_HPP
#define PHD_TEXT_BASIC_TEXT_VIEW_HPP

#include <phd/text/basic_c_string_view.hpp>
#include <phd/text/normalization.hpp>
#include <phd/text/code_unit.hpp>

namespace phd {

	template <typename __Encoding, typename __NormalizationForm = nfkc, typename __Iterable = basic_c_string_view<encoding_code_unit_t<__Encoding>>>
	class basic_text_view {
	public:
		using encoding_type = __Encoding;
		using iterable_type = __Iterable;

	private:
		iterable_type __mstorage;

	public:
		iterable_type&& storage() && {
			return std::move(__mstorage);
		}

		const iterable_type& storage() const& {
			return __mstorage;
		}

		iterable_type& storage() & {
			return __mstorage;
		}
	};

} // namespace phd

#endif // PHD_TEXT_BASIC_TEXT_VIEW_HPP
