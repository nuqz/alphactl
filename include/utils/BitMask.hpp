#pragma once

#include <type_traits>

/**
 * @brief A utility template class for bit mask operations.
 *
 * @tparam T Unsigned integer type from uint8_t to uint64_t.
 *
 * This class provides a type-safe interface for manipulating individual bits
 * and performing common bit mask operations. All methods are constexpr and
 * noexcept, allowing compile-time evaluation and guaranteed no-exception
 * behavior.
 */
template <typename T>
class BitMask
{
public:
    static_assert(std::is_integral_v<T> && std::is_unsigned_v<T>,
                  "BitMask requires an unsigned integral type");

    /**
     * @brief Default constructor. Initializes the mask to zero.
     */
    constexpr BitMask() noexcept : _value(0) {}

    /**
     * @brief Constructs a BitMask with the specified initial value.
     * @param value Initial value for the mask.
     */
    constexpr explicit BitMask(T value) noexcept : _value{value} {}

    /**
     * @brief Sets the bit at the specified position to 1.
     * @param position Bit position (0-based).
     * @return Reference to this BitMask for method chaining.
     */
    constexpr BitMask &set(std::size_t position) noexcept
    {
        _value |= (static_cast<T>(1) << position);
        return *this;
    }

    /**
     * @brief Clears the bit at the specified position to 0.
     * @param position Bit position (0-based).
     * @return Reference to this BitMask for method chaining.
     */
    constexpr BitMask &clear(std::size_t position) noexcept
    {
        _value &= ~(static_cast<T>(1) << position);
        return *this;
    }

    /**
     * @brief Toggles the bit at the specified position.
     * @param position Bit position (0-based).
     * @return Reference to this BitMask for method chaining.
     */
    constexpr BitMask &toggle(std::size_t position) noexcept
    {
        _value ^= (static_cast<T>(1) << position);
        return *this;
    }

    /**
     * @brief Checks if the bit at the specified position is set.
     * @param position Bit position (0-based).
     * @return true if the bit is set, false otherwise.
     */
    [[nodiscard]] constexpr bool test(std::size_t position) const noexcept
    {
        return (_value & (static_cast<T>(1) << position)) != 0;
    }

    /**
     * @brief Checks if all bits in the mask are set to 1.
     * @return true if all bits are set, false otherwise.
     */
    [[nodiscard]] constexpr bool all() const noexcept
    {
        return _value == static_cast<T>(~static_cast<std::make_unsigned_t<decltype(_value)>>(0));
    }

    /**
     * @brief Checks if any bit in the mask is set to 1.
     * @return true if at least one bit is set, false otherwise.
     */
    [[nodiscard]] constexpr bool any() const noexcept
    {
        return _value != 0;
    }

    /**
     * @brief Checks if no bits in the mask are set to 1.
     * @return true if all bits are zero, false otherwise.
     */
    [[nodiscard]] constexpr bool none() const noexcept
    {
        return _value == 0;
    }

    /**
     * @brief Checks if exactly one bit in the mask is set to 1.
     * @return true if exactly one bit is set, false otherwise.
     *
     * Uses the bit manipulation idiom: (x != 0) && ((x & (x - 1)) == 0)
     */
    [[nodiscard]] constexpr bool single() const noexcept
    {
        return _value != 0 && (_value & (_value - 1)) == 0;
    }

    /**
     * @brief Resets all bits in the mask to 0.
     * @return Reference to this BitMask for method chaining.
     */
    constexpr BitMask &reset() noexcept
    {
        _value = 0;
        return *this;
    }

    /**
     * @brief Sets all bits in the mask to 1.
     * @return Reference to this BitMask for method chaining.
     */
    constexpr BitMask &fill() noexcept
    {
        _value = ~static_cast<T>(0);
        return *this;
    }

    /**
     * @brief Explicit conversion to the underlying type T.
     * @return The underlying value of the mask.
     */
    [[nodiscard]] constexpr explicit operator T() const noexcept
    {
        return _value;
    }

private:
    T _value;
};
