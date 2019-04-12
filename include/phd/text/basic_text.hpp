#pragma once

#ifndef PHD_TEXT_BASIC_TEXT_HPP
#define PHD_TEXT_BASIC_TEXT_HPP

#include <phd/text/basic_text_view.hpp>
#include <phd/text/text_forward.hpp>
#include <phd/text/text_traits.hpp>

#include <string>

namespace phd {

	template <typename __Encoding, typename __NormalizationForm = nfkc, typename __Container = std::basic_string<encoding_code_unit_t<__Encoding>>>
	class basic_text : public basic_text_view<__Encoding, __NormalizationForm, __Container> {
	};

} // namespace phd

#endif // PHD_TEXT_BASIC_TEXT_HPP
