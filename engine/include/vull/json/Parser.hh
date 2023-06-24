#pragma once

#include <vull/json/Tree.hh> // IWYU pragma: keep
#include <vull/support/Result.hh>
#include <vull/support/StringView.hh>

namespace vull::json {

Result<Value, JsonError> parse(StringView source);

} // namespace vull::json
