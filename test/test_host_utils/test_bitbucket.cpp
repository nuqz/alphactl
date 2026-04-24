#include <unity.h>

#include "utils/BitBucket.hpp"

void test_bitbucket_default_constructor()
{
    BitBucket<uint8_t> bucket;
    // Fresh bucket should have all bits available (none in use)
    // We can't directly check the mask, but we can try to acquire
}

void test_bitbucket_acquire_single()
{
    BitBucket<uint8_t> bucket;

    // Acquire position 3
    TEST_ASSERT_EQUAL(BitBucketBase::Result::Ok, bucket.acquire(3));

    // Try to acquire same position again - should fail (InUse)
    TEST_ASSERT_EQUAL(BitBucketBase::Result::InUse, bucket.acquire(3));

    // Acquire different position - should succeed
    TEST_ASSERT_EQUAL(BitBucketBase::Result::Ok, bucket.acquire(5));
}

void test_bitbucket_release_single()
{
    BitBucket<uint8_t> bucket;

    // Acquire then release
    TEST_ASSERT_EQUAL(BitBucketBase::Result::Ok, bucket.acquire(3));
    TEST_ASSERT_EQUAL(BitBucketBase::Result::Ok, bucket.release(3));

    // Now should be able to acquire again
    TEST_ASSERT_EQUAL(BitBucketBase::Result::Ok, bucket.acquire(3));
}

void test_bitbucket_release_not_in_use()
{
    BitBucket<uint8_t> bucket;

    // Try to release without acquiring first
    TEST_ASSERT_EQUAL(BitBucketBase::Result::NotInUse, bucket.release(3));
}

void test_bitbucket_invalid_position_acquire()
{
    BitBucket<uint8_t> bucket;

    // Position 8 is invalid for uint8_t (0-7 are valid)
    TEST_ASSERT_EQUAL(BitBucketBase::Result::InvalidPosition, bucket.acquire(8));
    TEST_ASSERT_EQUAL(BitBucketBase::Result::InvalidPosition, bucket.acquire(255));
}

void test_bitbucket_invalid_position_release()
{
    BitBucket<uint8_t> bucket;

    // Position 8 is invalid for uint8_t
    TEST_ASSERT_EQUAL(BitBucketBase::Result::InvalidPosition, bucket.release(8));
}

void test_bitbucket_explicit_constructor()
{
    // Create bucket with some bits already set (in use)
    BitBucket<uint8_t> bucket(0x04); // Bit 2 is set

    // Position 2 should be InUse
    TEST_ASSERT_EQUAL(BitBucketBase::Result::InUse, bucket.acquire(2));

    // Other positions should be available
    TEST_ASSERT_EQUAL(BitBucketBase::Result::Ok, bucket.acquire(0));
    TEST_ASSERT_EQUAL(BitBucketBase::Result::Ok, bucket.acquire(7));
}

void test_bitbucket_all_positions()
{
    BitBucket<uint8_t> bucket;

    // Acquire all 8 positions
    for (size_t i = 0; i < 8; i++)
    {
        TEST_ASSERT_EQUAL(BitBucketBase::Result::Ok, bucket.acquire(i));
    }

    // All should now be InUse
    for (size_t i = 0; i < 8; i++)
    {
        TEST_ASSERT_EQUAL(BitBucketBase::Result::InUse, bucket.acquire(i));
    }
}

void test_bitbucket_release_all()
{
    BitBucket<uint8_t> bucket;

    // Acquire all
    for (size_t i = 0; i < 8; i++)
    {
        TEST_ASSERT_EQUAL(BitBucketBase::Result::Ok, bucket.acquire(i));
    }

    // Release all
    for (size_t i = 0; i < 8; i++)
    {
        TEST_ASSERT_EQUAL(BitBucketBase::Result::Ok, bucket.release(i));
    }

    // All should be available again
    for (size_t i = 0; i < 8; i++)
    {
        TEST_ASSERT_EQUAL(BitBucketBase::Result::Ok, bucket.acquire(i));
    }
}

void test_bitbucket_uint16()
{
    BitBucket<uint16_t> bucket;

    // Test some positions in 16-bit range
    TEST_ASSERT_EQUAL(BitBucketBase::Result::Ok, bucket.acquire(0));
    TEST_ASSERT_EQUAL(BitBucketBase::Result::Ok, bucket.acquire(15));
    TEST_ASSERT_EQUAL(BitBucketBase::Result::InvalidPosition, bucket.acquire(16));
}

void test_bitbucket_custom_numbits()
{
    // Bucket with only 4 bits available
    BitBucket<uint8_t, 4> bucket;

    // Valid positions 0-3
    TEST_ASSERT_EQUAL(BitBucketBase::Result::Ok, bucket.acquire(0));
    TEST_ASSERT_EQUAL(BitBucketBase::Result::Ok, bucket.acquire(3));

    // Invalid positions >= 4
    TEST_ASSERT_EQUAL(BitBucketBase::Result::InvalidPosition, bucket.acquire(4));
    TEST_ASSERT_EQUAL(BitBucketBase::Result::InvalidPosition, bucket.release(4));
}

void run_test_bitbucket()
{
    RUN_TEST(test_bitbucket_default_constructor);
    RUN_TEST(test_bitbucket_acquire_single);
    RUN_TEST(test_bitbucket_release_single);
    RUN_TEST(test_bitbucket_release_not_in_use);
    RUN_TEST(test_bitbucket_invalid_position_acquire);
    RUN_TEST(test_bitbucket_invalid_position_release);
    RUN_TEST(test_bitbucket_explicit_constructor);
    RUN_TEST(test_bitbucket_all_positions);
    RUN_TEST(test_bitbucket_release_all);
    RUN_TEST(test_bitbucket_uint16);
    RUN_TEST(test_bitbucket_custom_numbits);
}
