#pragma once

#include <cstdint>
#include <type_traits>

#include "BitMask.hpp"

/**
 * @brief A utility template class for tracking bit availability (free/occupied).
 *
 * @tparam T Unsigned integer type for the underlying bit mask.
 *
 * BitBucket provides a semantic layer over BitMask to track resource availability,
 * where each bit represents a resource slot (e.g., GPIO pin). A set bit (1) means
 * the resource is occupied, and a cleared bit (0) means it's free.
 */
template <typename T>
class BitBucket
{
public:
    /**
     * @brief Maximum number of bits that can be managed by this BitBucket.
     */
    static constexpr std::size_t kMaxBits = sizeof(T) * 8;

    /**
     * @brief Default constructor. Initializes all bits as free (zero).
     */
    constexpr BitBucket() noexcept : _mask{} {}

    /**
     * @brief Constructs a BitBucket with the specified initial occupancy mask.
     * @param value Initial occupancy mask (1 = occupied, 0 = free).
     */
    constexpr explicit BitBucket(T value) noexcept : _mask{value} {}

    /**
     * @brief Marks the bit at the specified position as occupied.
     * @param position Bit position (0-based).
     * @return true if successfully marked as occupied, false if already occupied.
     */
    constexpr bool acquire(std::size_t position) noexcept
    {
        if (position >= kMaxBits || isOccupied(position))
        {
            return false;
        }
        _mask.set(position);
        return true;
    }

    /**
     * @brief Marks the bit at the specified position as free.
     * @param position Bit position (0-based).
     * @return true if successfully marked as free, false if already free.
     */
    constexpr bool release(std::size_t position) noexcept
    {
        if (position >= kMaxBits || isFree(position))
        {
            return false;
        }
        _mask.clear(position);
        return true;
    }

    /**
     * @brief Checks if the bit at the specified position is free.
     * @param position Bit position (0-based).
     * @return true if the bit is free (0), false if occupied (1).
     */
    [[nodiscard]] constexpr bool isFree(std::size_t position) const noexcept
    {
        if (position >= kMaxBits)
        {
            return false;
        }
        return !_mask.test(position);
    }

    /**
     * @brief Checks if the bit at the specified position is occupied.
     * @param position Bit position (0-based).
     * @return true if the bit is occupied (1), false if free (0).
     */
    [[nodiscard]] constexpr bool isOccupied(std::size_t position) const noexcept
    {
        if (position >= kMaxBits)
        {
            return false;
        }
        return _mask.test(position);
    }

private:
    BitMask<T> _mask;
};
