##ifndef CTZ_SAFE_CCTYPE_HPP
#define CTZ_SAFE_CCTYPE_HPP

// safe_cctype.hpp — UB‑free helpers for <cctype>
// 
// Why: All <cctype> functions (e.g., std::toupper, std::isalpha) have
// undefined behavior if called with a value that is neither EOF nor
// representable as unsigned char. These wrappers enforce the cast so you
// can use plain `char` safely and idiomatically.
//
// Drop this header into your project and include it wherever you touch
// characters or use algorithms over std::string.
//
// Locale: Behavior follows the currently installed C locale
// (see std::setlocale). In the default "C" locale, only ASCII a–z/A–Z
// case mappings apply.

#include <cctype>
#include <algorithm>
#include <iterator>
#include <string>
#include <string_view>
#include <type_traits>

namespace ctz::safe {

// ------------------------------
// Character transforms (single)
// ------------------------------
[[nodiscard]] inline char to_upper(char ch) noexcept {
    return static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
}

[[nodiscard]] inline char to_lower(char ch) noexcept {
    return static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
}

// ------------------------------
// Character classifications
// Return type is bool-like (int in <cctype>), but we expose bool.
// ------------------------------
[[nodiscard]] inline bool is_alpha(char ch) noexcept {
    return std::isalpha(static_cast<unsigned char>(ch)) != 0;
}
[[nodiscard]] inline bool is_digit(char ch) noexcept {
    return std::isdigit(static_cast<unsigned char>(ch)) != 0;
}
[[nodiscard]] inline bool is_alnum(char ch) noexcept {
    return std::isalnum(static_cast<unsigned char>(ch)) != 0;
}
[[nodiscard]] inline bool is_space(char ch) noexcept {
    return std::isspace(static_cast<unsigned char>(ch)) != 0;
}
[[nodiscard]] inline bool is_cntrl(char ch) noexcept {
    return std::iscntrl(static_cast<unsigned char>(ch)) != 0;
}
[[nodiscard]] inline bool is_punct(char ch) noexcept {
    return std::ispunct(static_cast<unsigned char>(ch)) != 0;
}
[[nodiscard]] inline bool is_print(char ch) noexcept {
    return std::isprint(static_cast<unsigned char>(ch)) != 0;
}
[[nodiscard]] inline bool is_graph(char ch) noexcept {
    return std::isgraph(static_cast<unsigned char>(ch)) != 0;
}
[[nodiscard]] inline bool is_xdigit(char ch) noexcept {
    return std::isxdigit(static_cast<unsigned char>(ch)) != 0;
}

// ------------------------------
// In-place transforms over ranges/containers
// ------------------------------
// Overload for std::string
inline void to_upper_inplace(std::string& s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
}
inline void to_lower_inplace(std::string& s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
}

// Generic iterator pair (works with vector<char>, string, etc.)
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
// using ctz::safe::to_upper;          // single-char
// using ctz::safe::to_upper_inplace;  // string & iterators
// using ctz::safe::to_upper_copy;     // returns std::string
// bool a = ctz::safe::is_alpha(ch);   // classification
//
// NOTE: Behavior follows current C locale (std::setlocale). If you need
// Unicode case mapping and classification, use ICU, Boost.Text, or C++23
// <text> (when available in your toolchain) instead of <cctype>.

#endif // CTZ_SAFE_CCTYPE_HPP

