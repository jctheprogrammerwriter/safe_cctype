#ifndef CTZ_SAFE_CCTYPE_HPP
template <class It>
inline void to_upper_inplace(It first, It last) {
using Char = std::remove_cv_t<std::remove_reference_t<decltype(*first)>>;
static_assert(std::is_same_v<Char, char>,
"to_upper_inplace(It,It) expects iterators over char");
std::transform(first, last, first,
[](unsigned char c) { return static_cast<char>(std::toupper(c)); });
}


template <class It>
inline void to_lower_inplace(It first, It last) {
using Char = std::remove_cv_t<std::remove_reference_t<decltype(*first)>>;
static_assert(std::is_same_v<Char, char>,
"to_lower_inplace(It,It) expects iterators over char");
std::transform(first, last, first,
[](unsigned char c) { return static_cast<char>(std::tolower(c)); });
}


// Copying transforms (return a new string)
[[nodiscard]] inline std::string to_upper_copy(std::string_view sv) {
std::string out(sv);
to_upper_inplace(out);
return out;
}
[[nodiscard]] inline std::string to_lower_copy(std::string_view sv) {
std::string out(sv);
to_lower_inplace(out);
return out;
}


// ---------------------------------
// Algorithm-friendly functor objects
// ---------------------------------
struct ToUpper {
[[nodiscard]] char operator()(unsigned char c) const noexcept {
return static_cast<char>(std::toupper(c));
}
};
struct ToLower {
[[nodiscard]] char operator()(unsigned char c) const noexcept {
return static_cast<char>(std::tolower(c));
}
};


// Example:
// std::transform(s.begin(), s.end(), s.begin(), ctz::safe::ToUpper{});


// ---------------------------------
// ASCII-only fast path (optional)
// ---------------------------------
// These helpers avoid locale calls if you KNOW your data is ASCII.
// They do not consider locale; they only map 'a'..'z' and 'A'..'Z'.
[[nodiscard]] constexpr char ascii_to_upper(char ch) noexcept {
return (ch >= 'a' && ch <= 'z') ? static_cast<char>(ch - 'a' + 'A') : ch;
}
[[nodiscard]] constexpr char ascii_to_lower(char ch) noexcept {
return (ch >= 'A' && ch <= 'Z') ? static_cast<char>(ch - 'A' + 'a') : ch;
}


} // namespace ctz::safe


// ------------------------------
// Quick usage guide
// ------------------------------
// #include "safe_cctype.hpp"
// using ctz::safe::to_upper; // single-char
// using ctz::safe::to_upper_inplace; // string & iterators
// using ctz::safe::to_upper_copy; // returns std::string
// bool a = ctz::safe::is_alpha(ch); // classification
//
// NOTE: Behavior follows current C locale (std::setlocale). If you need
// Unicode case mapping and classification, use ICU, Boost.Text, or C++23
// <text> (when available in your toolchain) instead of <cctype>.


#endif // CTZ_SAFE_CCTYPE_HPP
