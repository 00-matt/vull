#include <vull/support/StringBuilder.hh>

#include <vull/maths/Common.hh>
#include <vull/support/Array.hh>
#include <vull/support/Span.hh>
#include <vull/support/String.hh>
#include <vull/support/StringView.hh>
#include <vull/support/Vector.hh>

#include <stdint.h>
#include <stdio.h>

namespace vull {

void StringBuilder::append_single(double arg, const char *) {
    // TODO: Own implementation.
    Array<char, 20> buf{};
    int length = snprintf(buf.data(), buf.size(), "%f", arg); // NOLINT
    m_buffer.extend(StringView(buf.data(), vull::min(buf.size(), static_cast<uint32_t>(length))));
}

void StringBuilder::append_single(size_t arg, const char *opts) {
    const size_t base = opts[0] == 'h' ? 16 : 10;
    Array<char, 30> buf{};
    uint8_t len = 0;
    do {
        auto digit = static_cast<char>(arg % base);
        buf[len++] = digit < 10 ? '0' + digit : 'a' + digit - 10; // NOLINT
        arg /= base;
    } while (arg > 0);

    if (const char pad = opts[1]; pad != '\0') {
        const char pad_char = opts[2] != '\0' ? opts[2] : '0';
        for (uint8_t i = len; i < pad - '0'; i++) {
            buf[len++] = pad_char;
        }
    }
    if (opts[0] == 'h') {
        buf[len++] = 'x';
        buf[len++] = '0';
    }
    for (uint8_t i = len; i > 0; i--) {
        m_buffer.push(buf[i - 1]);
    }
}

void StringBuilder::append_single(StringView arg, const char *) {
    m_buffer.extend(arg);
}

void StringBuilder::append(char ch) {
    m_buffer.push(ch);
}

String StringBuilder::build() {
    m_buffer.push('\0');
    auto buffer = m_buffer.take_all();
    return String::move_raw(buffer.data(), buffer.size() - 1);
}

String StringBuilder::build_copy() const {
    return String::copy_raw(m_buffer.data(), m_buffer.size());
}

} // namespace vull
