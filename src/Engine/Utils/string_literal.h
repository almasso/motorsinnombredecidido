#ifndef STRING_LITERAL_H
#define STRING_LITERAL_H

#include <algorithm>

template <auto N>
struct string_literal {
    constexpr string_literal(const char (&str)[N]) {
        std::copy_n(str, N, value);
    }

    char value[N];
};

#endif //STRING_LITERAL_H
