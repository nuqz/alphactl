#pragma once

#include <cstdint>

#include "BitMask.hpp"

struct BitBucketBase
{
    enum class Result : uint8_t
    {
        Ok = 0,
        InUse,
        NotInUse,
        InvalidPosition,
    };
};

template <typename T, size_t NumBits = sizeof(T) * CHAR_BIT>
class BitBucket : BitBucketBase
{
public:
    constexpr BitBucket() noexcept : _mask{} {}

    constexpr explicit BitBucket(T value) noexcept : _mask{value} {}

    [[nodiscard]] constexpr Result acquire(size_t position) noexcept
    {
        return modify_if(position, true, Result::InUse, &TBitMask::set);
    }

    [[nodiscard]] constexpr Result release(size_t position) noexcept
    {
        return modify_if(position, false, Result::NotInUse, &TBitMask::clear);
    }

private:
    using TBitMask = BitMask<T, NumBits>;

    TBitMask _mask;

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
