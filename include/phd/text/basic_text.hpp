#pragma once

#ifndef PHD_TEXT_BASIC_TEXT_HPP
#define PHD_TEXT_BASIC_TEXT_HPP

#include <phd/text/basic_text_view.hpp>
#include <phd/text/text_forward.hpp>
#include <phd/text/text_traits.hpp>

#include <string>

namespace phd {

	template <typename Encoding, typename NormalizationForm = nfkc, typename Container = std::basic_string<code_unit_of_t<Encoding>>>
	class basic_text : public basic_text_view<Encoding, NormalizationForm, Container> {
	};

} // namespace phd

#endif // PHD_TEXT_BASIC_TEXT_HPP
