#pragma once

#include <cstdint>
#include <vector>

namespace countdown {

using Value = std::int64_t;

std::uint64_t absoluteDifference(Value lhs, Value rhs) noexcept;
Value solve(std::vector<Value> &numbers, Value target, Value nearest);

} // namespace countdown
