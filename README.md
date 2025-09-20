# safe_cctype
a compact, UB-free wrapper set for &lt;cctype> with:  Safe single-char transforms: to_upper, to_lower  Safe classifiers: is_alpha, is_digit, is_alnum, is_space, etc.  In-place and copying string transforms  Iterator-based overloads and algorithm-friendly functors  Optional ASCII-only constexpr fast paths  Clear usage notes about locale behavior
