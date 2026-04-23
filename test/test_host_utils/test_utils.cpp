#include <unity.h>

#include "utils/BitMask.hpp"
#include "utils/BitBucket.hpp"

void test_default_constructor()
{
    BitMask<uint8_t> mask;
    TEST_ASSERT_FALSE(mask.any());
    TEST_ASSERT_TRUE(mask.none());
}

void test_explicit_constructor()
{
    BitMask<uint8_t> mask(0b10101010);
    TEST_ASSERT_TRUE(mask.test(1));
    TEST_ASSERT_TRUE(mask.test(3));
    TEST_ASSERT_TRUE(mask.test(5));
    TEST_ASSERT_TRUE(mask.test(7));
    TEST_ASSERT_FALSE(mask.test(0));
}

void test_set()
{
    BitMask<uint8_t> mask;
    mask.set(3);
    TEST_ASSERT_TRUE(mask.test(3));
    mask.set(5);
    TEST_ASSERT_TRUE(mask.test(5));
}

void test_clear()
{
    BitMask<uint8_t> mask(0xFF);
    mask.clear(3);
    TEST_ASSERT_FALSE(mask.test(3));
    mask.clear(5);
    TEST_ASSERT_FALSE(mask.test(5));
}

void test_toggle()
{
    BitMask<uint8_t> mask;
    mask.toggle(3);
    TEST_ASSERT_TRUE(mask.test(3));
    mask.toggle(3);
    TEST_ASSERT_FALSE(mask.test(3));
}

void test_all()
{
    BitMask<uint8_t> mask_empty;
    TEST_ASSERT_FALSE(mask_empty.all());

    BitMask<uint8_t> mask_full(0xFF);
    TEST_ASSERT_TRUE(mask_full.all());

    BitMask<uint8_t> mask_partial(0x0F);
    TEST_ASSERT_FALSE(mask_partial.all());

    // Test with explicit fill
    BitMask<uint8_t> mask_filled;
    mask_filled.fill();
    TEST_ASSERT_TRUE(mask_filled.all());
}

void test_any()
{
    BitMask<uint8_t> mask_empty;
    TEST_ASSERT_FALSE(mask_empty.any());

    BitMask<uint8_t> mask_one(0x01);
    TEST_ASSERT_TRUE(mask_one.any());

    BitMask<uint8_t> mask_full(0xFF);
    TEST_ASSERT_TRUE(mask_full.any());
}

void test_none()
{
    BitMask<uint8_t> mask_empty;
    TEST_ASSERT_TRUE(mask_empty.none());

    BitMask<uint8_t> mask_one(0x01);
    TEST_ASSERT_FALSE(mask_one.none());
}

void test_single_zero()
{
    BitMask<uint8_t> mask(0);
    TEST_ASSERT_FALSE(mask.single());
}

void test_single_one_bit()
{
    TEST_ASSERT_TRUE(BitMask<uint8_t>(0x01).single());
    TEST_ASSERT_TRUE(BitMask<uint8_t>(0x02).single());
    TEST_ASSERT_TRUE(BitMask<uint8_t>(0x04).single());
    TEST_ASSERT_TRUE(BitMask<uint8_t>(0x80).single());
}

void test_single_multiple_bits()
{
    TEST_ASSERT_FALSE(BitMask<uint8_t>(0x03).single());
    TEST_ASSERT_FALSE(BitMask<uint8_t>(0x0F).single());
    TEST_ASSERT_FALSE(BitMask<uint8_t>(0xFF).single());
}

void test_reset()
{
    BitMask<uint8_t> mask(0xFF);
    mask.reset();
    TEST_ASSERT_TRUE(mask.none());
    TEST_ASSERT_FALSE(mask.any());
}

void test_fill()
{
    BitMask<uint8_t> mask;
    mask.fill();
    TEST_ASSERT_TRUE(mask.all());

    // Also verify the value is 0xFF for uint8_t
    TEST_ASSERT_EQUAL(0xFF, static_cast<uint8_t>(mask));
}

void test_method_chaining()
{
    BitMask<uint8_t> mask;
    mask.set(1).set(3).clear(1).toggle(5);
    TEST_ASSERT_FALSE(mask.test(1));
    TEST_ASSERT_TRUE(mask.test(3));
    TEST_ASSERT_TRUE(mask.test(5));
}

void test_uint16()
{
    BitMask<uint16_t> mask(0xAAAA);
    TEST_ASSERT_TRUE(mask.test(1));
    TEST_ASSERT_TRUE(mask.test(15));
    TEST_ASSERT_FALSE(mask.test(0));
}

void test_uint32()
{
    BitMask<uint32_t> mask(0xAAAAAAAA);
    TEST_ASSERT_TRUE(mask.test(1));
    TEST_ASSERT_TRUE(mask.test(31));
    TEST_ASSERT_FALSE(mask.test(0));
}

void test_uint64()
{
    BitMask<uint64_t> mask(0xAAAAAAAAAAAAAAAAULL);
    TEST_ASSERT_TRUE(mask.test(1));
    TEST_ASSERT_TRUE(mask.test(63));
    TEST_ASSERT_FALSE(mask.test(0));
}

void test_explicit_conversion()
{
    BitMask<uint8_t> mask(0b10101010);
    uint8_t value = static_cast<uint8_t>(mask);
    TEST_ASSERT_EQUAL(0xAA, value);
}

void test_single_uint64_high_bit()
{
    BitMask<uint64_t> mask(1ULL << 63);
    TEST_ASSERT_TRUE(mask.single());
}

void test_bitbucket_default_constructor()
{
    BitBucket<uint8_t> bucket;
    TEST_ASSERT_TRUE(bucket.isFree(0));
    TEST_ASSERT_TRUE(bucket.isFree(7));
    TEST_ASSERT_FALSE(bucket.isOccupied(0));
    TEST_ASSERT_FALSE(bucket.isOccupied(7));
}

void test_bitbucket_explicit_constructor()
{
    BitBucket<uint8_t> bucket(0b10101010);
    TEST_ASSERT_FALSE(bucket.isFree(1));
    TEST_ASSERT_FALSE(bucket.isFree(3));
    TEST_ASSERT_FALSE(bucket.isFree(5));
    TEST_ASSERT_FALSE(bucket.isFree(7));
    TEST_ASSERT_TRUE(bucket.isFree(0));
    TEST_ASSERT_TRUE(bucket.isFree(2));
    TEST_ASSERT_TRUE(bucket.isFree(4));
    TEST_ASSERT_TRUE(bucket.isFree(6));
}

void test_bitbucket_acquire_ok()
{
    BitBucket<uint8_t> bucket;
    TEST_ASSERT_EQUAL(BitBucketBase::AcquireResult::Ok, bucket.acquire(3));
    TEST_ASSERT_FALSE(bucket.isFree(3));
    TEST_ASSERT_TRUE(bucket.isOccupied(3));
}

void test_bitbucket_acquire_in_use()
{
    BitBucket<uint8_t> bucket;
    TEST_ASSERT_EQUAL(BitBucketBase::AcquireResult::Ok, bucket.acquire(3));
    TEST_ASSERT_EQUAL(BitBucketBase::AcquireResult::InUse, bucket.acquire(3));
}

void test_bitbucket_acquire_invalid()
{
    BitBucket<uint8_t> bucket;
    TEST_ASSERT_EQUAL(BitBucketBase::AcquireResult::Invalid, bucket.acquire(8));
}

void test_bitbucket_release_ok()
{
    BitBucket<uint8_t> bucket(0xFF);
    TEST_ASSERT_EQUAL(BitBucketBase::ReleaseResult::Ok, bucket.release(3));
    TEST_ASSERT_TRUE(bucket.isFree(3));
    TEST_ASSERT_FALSE(bucket.isOccupied(3));
}

void test_bitbucket_release_not_in_use()
{
    BitBucket<uint8_t> bucket(0xFF);
    TEST_ASSERT_EQUAL(BitBucketBase::ReleaseResult::Ok, bucket.release(3));
    TEST_ASSERT_EQUAL(BitBucketBase::ReleaseResult::NotInUse, bucket.release(3));
}

void test_bitbucket_release_invalid()
{
    BitBucket<uint8_t> bucket;
    TEST_ASSERT_EQUAL(BitBucketBase::ReleaseResult::Invalid, bucket.release(8));
}

void test_bitbucket_acquire_release_sequence()
{
    BitBucket<uint8_t> bucket;

    // Acquire some bits
    TEST_ASSERT_EQUAL(BitBucketBase::AcquireResult::Ok, bucket.acquire(0));
    TEST_ASSERT_EQUAL(BitBucketBase::AcquireResult::Ok, bucket.acquire(2));
    TEST_ASSERT_EQUAL(BitBucketBase::AcquireResult::Ok, bucket.acquire(4));

    TEST_ASSERT_FALSE(bucket.isFree(0));
    TEST_ASSERT_FALSE(bucket.isFree(2));
    TEST_ASSERT_FALSE(bucket.isFree(4));
    TEST_ASSERT_TRUE(bucket.isOccupied(0));
    TEST_ASSERT_TRUE(bucket.isOccupied(2));
    TEST_ASSERT_TRUE(bucket.isOccupied(4));

    // Release one
    TEST_ASSERT_EQUAL(BitBucketBase::ReleaseResult::Ok, bucket.release(2));
    TEST_ASSERT_TRUE(bucket.isFree(2));
    TEST_ASSERT_FALSE(bucket.isOccupied(2));

    // Re-acquire
    TEST_ASSERT_EQUAL(BitBucketBase::AcquireResult::Ok, bucket.acquire(2));
    TEST_ASSERT_FALSE(bucket.isFree(2));
    TEST_ASSERT_TRUE(bucket.isOccupied(2));
}

void test_bitbucket_uint16()
{
    BitBucket<uint16_t> bucket;
    TEST_ASSERT_EQUAL(BitBucketBase::AcquireResult::Ok, bucket.acquire(15));
    TEST_ASSERT_FALSE(bucket.isFree(15));
    TEST_ASSERT_TRUE(bucket.isOccupied(15));
}

void test_bitbucket_uint32()
{
    BitBucket<uint32_t> bucket;
    TEST_ASSERT_EQUAL(BitBucketBase::AcquireResult::Ok, bucket.acquire(31));
    TEST_ASSERT_FALSE(bucket.isFree(31));
    TEST_ASSERT_TRUE(bucket.isOccupied(31));
}

void test_bitbucket_uint64()
{
    BitBucket<uint64_t> bucket;
    TEST_ASSERT_EQUAL(BitBucketBase::AcquireResult::Ok, bucket.acquire(63));
    TEST_ASSERT_FALSE(bucket.isFree(63));
    TEST_ASSERT_TRUE(bucket.isOccupied(63));
}

void setUp() {}
void tearDown() {}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_default_constructor);
    RUN_TEST(test_explicit_constructor);
    RUN_TEST(test_set);
    RUN_TEST(test_clear);
    RUN_TEST(test_toggle);
    RUN_TEST(test_all);
    RUN_TEST(test_any);
    RUN_TEST(test_none);
    RUN_TEST(test_single_zero);
    RUN_TEST(test_single_one_bit);
    RUN_TEST(test_single_multiple_bits);
    RUN_TEST(test_reset);
    RUN_TEST(test_fill);
    RUN_TEST(test_method_chaining);
    RUN_TEST(test_uint16);
    RUN_TEST(test_uint32);
    RUN_TEST(test_uint64);
    RUN_TEST(test_explicit_conversion);
    RUN_TEST(test_single_uint64_high_bit);

    RUN_TEST(test_bitbucket_default_constructor);
    RUN_TEST(test_bitbucket_explicit_constructor);
    RUN_TEST(test_bitbucket_acquire_ok);
    RUN_TEST(test_bitbucket_acquire_in_use);
    RUN_TEST(test_bitbucket_acquire_invalid);
    RUN_TEST(test_bitbucket_release_ok);
    RUN_TEST(test_bitbucket_release_not_in_use);
    RUN_TEST(test_bitbucket_release_invalid);
    RUN_TEST(test_bitbucket_acquire_release_sequence);
    RUN_TEST(test_bitbucket_uint16);
    RUN_TEST(test_bitbucket_uint32);
    RUN_TEST(test_bitbucket_uint64);

    return UNITY_END();
}
