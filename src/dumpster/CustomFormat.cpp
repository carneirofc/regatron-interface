
#include <fmt/format.h>
#include <regatron/Readings.hpp>
#include <sstream>

template <> struct fmt::formatter<T_ErrorTree32> {
    // check https://fmt.dev/latest/api.html#formatting-user-defined-types

    // Presentation format: 'f' - fixed, 'e' - exponential.
    char presentation = 'f';

    // Parse format specifications ...
    constexpr auto parse(format_parse_context &ctx) {
        // Parse the presentation format and store it in the formatter:
        auto it = ctx.begin(), end = ctx.end();
        if (it != end && (*it == 'f' || *it == 'e'))
            presentation = *it++;

        // Check if reached the end of the range:
        if (it != end && *it != '}')
            throw format_error("invalid format");

        // Return an iterator past the end of the parsed range:
        return it;
    }

    // Formats the point p using the parsed format specification (presentation)
    // stored in this formatter.
    template <typename FormatContext>
    auto format(const T_ErrorTree32 &errorTree32, FormatContext &ctx) {
        // @fixme: !
        std::ostringstream oss;
        oss << "[" << errorTree32.group << ",";
        int index = 0;
        for (const auto &error : errorTree32.error) {
            oss << error;
            index++;
            if (index < 32)
                oss << ",";
        }
        oss << "]";
        return oss.str();
    }
};
