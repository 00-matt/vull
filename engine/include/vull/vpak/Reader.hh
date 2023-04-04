#pragma once

#include <vull/support/Optional.hh>
#include <vull/support/PerfectHasher.hh>
#include <vull/support/Result.hh>
#include <vull/support/Span.hh>
#include <vull/support/Stream.hh>
#include <vull/support/StreamError.hh>
#include <vull/support/StringView.hh>
#include <vull/support/UniquePtr.hh>
#include <vull/support/Vector.hh>
#include <vull/vpak/PackFile.hh> // IWYU pragma: keep

#include <stddef.h>
#include <stdint.h>

namespace vull {

class File;

} // namespace vull

namespace vull::vpak {

class ReadStream final : public Stream {
    const LargeSpan<uint8_t> m_data;
    size_t m_block_start;
    size_t m_compressed_size{0};
    size_t m_offset{0};

public:
    ReadStream(LargeSpan<uint8_t> data, size_t first_block);

    // TODO: override read_byte for a faster path.
    Result<size_t, StreamError> read(Span<void> data) override;
};

class Reader {
    LargeSpan<uint8_t> m_data;
    Vector<Entry> m_entries;
    PerfectHasher m_phf;

public:
    explicit Reader(File &&file);
    Reader(const Reader &) = delete;
    Reader(Reader &&) = delete;
    ~Reader();

    Reader &operator=(const Reader &) = delete;
    Reader &operator=(Reader &&) = delete;

    bool exists(StringView name) const;
    UniquePtr<ReadStream> open(StringView name) const;
    Optional<Entry> stat(StringView name) const;
    const Vector<Entry> &entries() const { return m_entries; }
};

} // namespace vull::vpak
