#pragma once

#include <climits>
#include <type_traits>

struct BitMaskBase
{
    enum class Result : uint8_t
    {
        Ok = 0,
        InvalidPosition,
    };
};

/**
 * @brief A utility template class for bit mask operations with optional
 *        bit-width constraint.
 *
 * @tparam T Unsigned integer type from uint8_t to uint64_t.
 * @tparam NumBits Maximum number of bits to use (default: sizeof(T) * 8).
 *
 * This class provides a type-safe interface for manipulating individual bits
 * and performing common bit mask operations.
 *
 * The NumBits template parameter allows restricting the usable bit range,
 * which is useful for embedded applications where only a subset of bits
 * represents valid hardware resources (e.g., GPIO pins).
 */
template <typename T, size_t NumBits = sizeof(T) * CHAR_BIT>
class BitMask : BitMaskBase
{
public:
    static_assert(std::is_integral_v<T> && std::is_unsigned_v<T>,
                  "BitMask<T, ...> requires an unsigned integral type");

    static_assert(NumBits > 0 && NumBits <= sizeof(T) * 8,
                  "BitMask<..., NumBits> must be between 1 and sizeof(T) * 8");

    /**
     * @brief Maximum number of bits available in this BitMask instance.
     */
    static constexpr size_t kMaxBits = sizeof(T) * CHAR_BIT;

    /**
     * @brief Active number of bits available in this BitMask instance.
     */
    static constexpr size_t kActiveBits = NumBits;

    /**
     * @brief Computes the full mask for a given number of bits.
     * @param num_bits Number of bits to include in the mask.
     * @return Mask with the lower num_bits set to 1.
     */
    [[nodiscard]] static constexpr T make_full_mask(size_t num_bits) noexcept
    {
        if (num_bits >= kMaxBits)
        {
            return ~static_cast<T>(0);
        }

        return (static_cast<T>(1) << num_bits) - static_cast<T>(1);
    }

    /**
     * @brief Default constructor. Initializes the mask to zero.
     */
    constexpr BitMask() noexcept : _value(0) {}

    /**
     * @brief Constructs a BitMask with the specified initial value.
     * @param value Initial value for the mask. Bits beyond NumBits are masked out.
     */
    constexpr explicit BitMask(T value) noexcept : _value{static_cast<T>(
                                                       value &
                                                       make_full_mask(kActiveBits))} {}

    /**
     * @brief Sets the bit at the specified position to 1.
     * @param position Bit position (0-based). Must be less than kActiveBits.
     * @return BitMask::Result::Ok if successful, BitMask::Result::InvalidPosition otherwise.
     */
    [[nodiscard]] constexpr Result set(size_t position) noexcept
    {
        if (position >= kActiveBits)
        {
            return Result::InvalidPosition;
        }

        _value |= (static_cast<T>(1) << position);
        return Result::Ok;
    }

    /**
     * @brief Clears the bit at the specified position to 0.
     * @param position Bit position (0-based). Must be less than kActiveBits.
     * @return BitMask::Result::Ok if successful, BitMask::Result::InvalidPosition otherwise.
     */
    [[nodiscard]] constexpr Result clear(size_t position) noexcept
    {
        if (position >= kActiveBits)
        {
            return Result::InvalidPosition;
        }

        _value &= ~(static_cast<T>(1) << position);
        return Result::Ok;
    }

    /**
     * @brief Toggles the bit at the specified position.
     * @param position Bit position (0-based). Must be less than kActiveBits.
     * @return BitMask::Result::Ok if successful, BitMask::Result::InvalidPosition otherwise.
     */
    [[nodiscard]] constexpr Result toggle(size_t position) noexcept
    {
        if (position >= kActiveBits)
        {
            return Result::InvalidPosition;
        }

        _value ^= (static_cast<T>(1) << position);
        return Result::Ok;
    }

    /**
     * @brief Checks if the bit at the specified position is set.
     * @param position Bit position (0-based). Must be less than kActiveBits.
     * @param[out] result Output parameter for the test result (true if set).
     * @return BitMask::Result::Ok if successful, BitMask::Result::InvalidPosition otherwise.
     */
    [[nodiscard]] constexpr Result test(size_t position, bool &result) const noexcept
    {
        if (position >= kActiveBits)
        {
            result = false;
            return Result::InvalidPosition;
        }

        result = (_value & (static_cast<T>(1) << position)) != 0;
        return Result::Ok;
    }

    /**
     * @brief Checks if all bits in the mask (within kActiveBits range) are set to 1.
     * @return true if all bits in the constrained range are set, false otherwise.
     */
    [[nodiscard]] constexpr bool all() const noexcept
    {
        constexpr T full_mask = make_full_mask(NumBits);
        return (_value & full_mask) == full_mask;
    }

    /**
     * @brief Checks if any bit in the mask (within kActiveBits range) is set to 1.
     * @return true if at least one bit in the constrained range is set, false otherwise.
     */
    [[nodiscard]] constexpr bool any() const noexcept
    {
        constexpr T full_mask = make_full_mask(NumBits);
        return (_value & full_mask) != 0;
    }

    /**
     * @brief Checks if no bits in the mask (within kActiveBits range) are set to 1.
     * @return true if all bits in the constrained range are zero, false otherwise.
     */
    [[nodiscard]] constexpr bool none() const noexcept
    {
        constexpr T full_mask = make_full_mask(NumBits);
        return (_value & full_mask) == 0;
    }

    /**
     * @brief Checks if exactly one bit in the mask (within kActiveBits range) is set to 1.
     * @return true if exactly one bit in the constrained range is set, false otherwise.
     *
     * Uses the bit manipulation idiom: (x != 0) && ((x & (x - 1)) == 0)
     */
    [[nodiscard]] constexpr bool single() const noexcept
    {
        constexpr T full_mask = make_full_mask(NumBits);
        T masked_value = _value & full_mask;
        return masked_value != 0 && (masked_value & (masked_value - 1)) == 0;
    }

    /**
     * @brief Resets all bits in the mask to 0.
     */
    constexpr void reset() noexcept
    {
        _value = 0;
    }

    /**
     * @brief Sets all bits in the mask to 1.
     */
    constexpr void fill() noexcept
    {
        _value = ~static_cast<T>(0);
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
