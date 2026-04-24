#pragma once

#include <cstdint>

#include "BitMask.hpp"

/**
 * @brief Base structure for BitBucket, containing common result codes.
 */
struct BitBucketBase
{
    /**
     * @brief Result codes for BitBucket operations.
     */
    enum class Result : uint8_t
    {
        Ok = 0,          ///< Operation completed successfully.
        InUse,           ///< Resource is already in use (for acquire).
        NotInUse,        ///< Resource is not in use (for release).
        InvalidPosition, ///< Requested bit position is out of valid range.
    };
};

/**
 * @brief A utility template class for managing bit-based resource allocation.
 *
 * @tparam T Unsigned integer type from uint8_t to uint64_t.
 * @tparam NumBits Maximum number of bits to use (default: sizeof(T) * 8).
 *
 * BitBucket provides a mechanism to track the usage state of individual bits,
 * where each bit represents a resource that can be either "acquired" (set to 1)
 * or "released" (set to 0). This is useful for managing pools of resources
 * such as DMA channels, timer instances, or GPIO pins in embedded systems.
 *
 * The class enforces state transitions:
 * - acquire(): Can only succeed if the bit is currently 0 (not in use).
 * - release(): Can only succeed if the bit is currently 1 (in use).
 *
 * The NumBits template parameter allows restricting the usable bit range,
 * which is useful when only a subset of bits represents valid resources.
 */
template <typename T, size_t NumBits = sizeof(T) * CHAR_BIT>
class BitBucket : BitBucketBase
{
public:
    /**
     * @brief Default constructor. Initializes all bits to 0 (released/not in use).
     */
    constexpr BitBucket() noexcept : _mask{} {}

    /**
     * @brief Constructs a BitBucket with the specified initial value.
     * @param value Initial value for the underlying mask. Bits beyond NumBits are masked out.
     *
     * This constructor allows pre-setting certain resources as "in use" at initialization.
     */
    constexpr explicit BitBucket(T value) noexcept : _mask{value} {}

    /**
     * @brief Attempts to acquire (set to 1) the resource at the specified position.
     * @param position Bit position (0-based) representing the resource to acquire.
     * @return Result::Ok if successfully acquired.
     * @return Result::InUse if the resource is already acquired.
     * @return Result::InvalidPosition if position is out of range.
     */
    [[nodiscard]] constexpr Result acquire(size_t position) noexcept
    {
        return modify_if(position, true, Result::InUse, &TBitMask::set);
    }

    /**
     * @brief Attempts to release (set to 0) the resource at the specified position.
     * @param position Bit position (0-based) representing the resource to release.
     * @return Result::Ok if successfully released.
     * @return Result::NotInUse if the resource was not acquired.
     * @return Result::InvalidPosition if position is out of range.
     */
    [[nodiscard]] constexpr Result release(size_t position) noexcept
    {
        return modify_if(position, false, Result::NotInUse, &TBitMask::clear);
    }

private:
    using TBitMask = BitMask<T, NumBits>;

    TBitMask _mask;

    /**
     * @brief Helper function to conditionally modify a bit based on its current state.
     *
     * @tparam Op Type of the member function pointer (set or clear).
     * @param position Bit position to modify.
     * @param expected_set Expected current state of the bit (true = set, false = clear).
     * @param error_if_wrong Error code to return if current state matches expected_set.
     * @param op Member function pointer to the modification operation.
     * @return Result::Ok if modification succeeded.
     * @return error_if_wrong if current state matches expected_set.
     * @return Result::InvalidPosition if position is invalid or operation fails.
     */
    template <typename Op>
    [[nodiscard]] constexpr Result modify_if(size_t position, bool expected_set, Result error_if_wrong, Op op) noexcept
    {
        bool is_set = false;
        if (_mask.test(position, is_set) != BitMaskBase::Result::Ok)
        {
            return Result::InvalidPosition;
        }

        if (is_set == expected_set)
        {
            return error_if_wrong;
        }

        if ((_mask.*op)(position) != BitMaskBase::Result::Ok)
        {
            return Result::InvalidPosition;
        }

        return Result::Ok;
    }
};
