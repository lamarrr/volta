/**
 * @file volta.h
 * @author Basit Ayantunde <rlamarrr@gmail.com>
 * @brief
 * @version  0.1
 * @date 2020-01-14
 *
 * @copyright Copyright (c) 2020
 *
 */
#ifndef VOLTA_VOLTA_H_
#define VOLTA_VOLTA_H_
#include <type_traits>

namespace volta {

// perform volatile read of the value from the memory location pointed to by
// [source], this prevents elision during compiler optimization. Commonly used
// for memory-mapped I/O.
//
//
// Safety:
// 1.   [source] must be non-nullptr
// 2.   [source] must be a valid memory location that can be read from
//
//
// Type Constraints:
// 1.   Only supports integral types, i.e: T can be either of bool, int8_t,
// uint8_t, char, int16_t, uint16_t, int32_t..., etc
//
//
// Example:
//            auto tick_count = 0U;
//            auto tick_count_snapshot = volta::volatile_read(&tick_count);
template <typename T>
inline T volatile_read(T const* source) noexcept {
  static_assert(std::is_integral<T>::value, "For integral types only");
  return *static_cast<T volatile const*>(source);
}

// perform volatile write of the value [value] to the memory location pointed to
// by [source], this prevents elision during compiler optimization. Commonly
// used for memory-mapped I/O.
//
//
// Safety:
// 1.   [source] must be non-nullptr
// 2.   [source] must be a valid memory location that can be read from
//
//
// Type Constraints:
// 1.   Only supports integral types, i.e: T can be either of bool, int8_t,
// uint8_t, char, int16_t, uint16_t, int32_t..., etc
//
//
// Example:
//            auto tick_count = 0U;
//            volta::volatile_write(&tick_count, 45);
template <typename T>
inline void volatile_write(T* dest, T const& value) noexcept {
  static_assert(std::is_integral<T>::value, "For integral types only");
  *static_cast<T volatile*>(dest) = value;
}

}  // namespace volta

#endif  // VOLTA_VOLTA_H_
