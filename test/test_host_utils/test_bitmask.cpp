#include <unity.h>

#include "utils/BitMask.hpp"

void test_bitmask_default_constructor()
{
    BitMask<uint8_t> mask;
    TEST_ASSERT_FALSE(mask.any());
    TEST_ASSERT_TRUE(mask.none());
}

void test_bitmask_explicit_constructor()
{
    BitMask<uint8_t> mask(0b10101010);
    bool result = false;

    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.test(1, result));
    TEST_ASSERT_TRUE(result);

    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.test(3, result));
    TEST_ASSERT_TRUE(result);

    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.test(5, result));
    TEST_ASSERT_TRUE(result);

    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.test(7, result));
    TEST_ASSERT_TRUE(result);

    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.test(0, result));
    TEST_ASSERT_FALSE(result);
}

void test_bitmask_set_ok()
{
    BitMask<uint8_t> mask;
    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.set(3));
    bool result = false;
    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.test(3, result));
    TEST_ASSERT_TRUE(result);

    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.set(5));
    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.test(5, result));
    TEST_ASSERT_TRUE(result);
}

void test_bitmask_set_invalid_position()
{
    BitMask<uint8_t> mask;
    TEST_ASSERT_EQUAL(BitMaskBase::Result::InvalidPosition, mask.set(8));
    TEST_ASSERT_EQUAL(BitMaskBase::Result::InvalidPosition, mask.set(255));
}

void test_bitmask_clear_ok()
{
    BitMask<uint8_t> mask(0xFF);
    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.clear(3));
    bool result = false;
    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.test(3, result));
    TEST_ASSERT_FALSE(result);

    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.clear(5));
    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.test(5, result));
    TEST_ASSERT_FALSE(result);
}

void test_bitmask_clear_invalid_position()
{
    BitMask<uint8_t> mask;
    TEST_ASSERT_EQUAL(BitMaskBase::Result::InvalidPosition, mask.clear(8));
    TEST_ASSERT_EQUAL(BitMaskBase::Result::InvalidPosition, mask.clear(255));
}

void test_bitmask_toggle_ok()
{
    BitMask<uint8_t> mask;
    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.toggle(3));
    bool result = false;
    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.test(3, result));
    TEST_ASSERT_TRUE(result);

    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.toggle(3));
    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.test(3, result));
    TEST_ASSERT_FALSE(result);
}

void test_bitmask_toggle_invalid_position()
{
    BitMask<uint8_t> mask;
    TEST_ASSERT_EQUAL(BitMaskBase::Result::InvalidPosition, mask.toggle(8));
    TEST_ASSERT_EQUAL(BitMaskBase::Result::InvalidPosition, mask.toggle(255));
}

void test_bitmask_test_invalid_position()
{
    BitMask<uint8_t> mask;
    bool result = true;
    TEST_ASSERT_EQUAL(BitMaskBase::Result::InvalidPosition, mask.test(8, result));
    TEST_ASSERT_FALSE(result); // result should be set to false on error

    TEST_ASSERT_EQUAL(BitMaskBase::Result::InvalidPosition, mask.test(255, result));
    TEST_ASSERT_FALSE(result);
}

void test_bitmask_all()
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

void test_bitmask_any()
{
    BitMask<uint8_t> mask_empty;
    TEST_ASSERT_FALSE(mask_empty.any());

    BitMask<uint8_t> mask_one(0x01);
    TEST_ASSERT_TRUE(mask_one.any());

    BitMask<uint8_t> mask_full(0xFF);
    TEST_ASSERT_TRUE(mask_full.any());
}

void test_bitmask_none()
{
    BitMask<uint8_t> mask_empty;
    TEST_ASSERT_TRUE(mask_empty.none());

    BitMask<uint8_t> mask_one(0x01);
    TEST_ASSERT_FALSE(mask_one.none());
}

void test_bitmask_single_zero()
{
    BitMask<uint8_t> mask(0);
    TEST_ASSERT_FALSE(mask.single());
}

void test_bitmask_single_one_bit()
{
    TEST_ASSERT_TRUE(BitMask<uint8_t>(0x01).single());
    TEST_ASSERT_TRUE(BitMask<uint8_t>(0x02).single());
    TEST_ASSERT_TRUE(BitMask<uint8_t>(0x04).single());
    TEST_ASSERT_TRUE(BitMask<uint8_t>(0x80).single());
}

void test_bitmask_single_multiple_bits()
{
    TEST_ASSERT_FALSE(BitMask<uint8_t>(0x03).single());
    TEST_ASSERT_FALSE(BitMask<uint8_t>(0x0F).single());
    TEST_ASSERT_FALSE(BitMask<uint8_t>(0xFF).single());
}

void test_bitmask_reset()
{
    BitMask<uint8_t> mask(0xFF);
    mask.reset();
    TEST_ASSERT_TRUE(mask.none());
    TEST_ASSERT_FALSE(mask.any());
}

void test_bitmask_fill()
{
    BitMask<uint8_t> mask;
    mask.fill();
    TEST_ASSERT_TRUE(mask.all());

    // Also verify the value is 0xFF for uint8_t
    TEST_ASSERT_EQUAL(0xFF, static_cast<uint8_t>(mask));
}

void test_bitmask_uint16()
{
    BitMask<uint16_t> mask(0xAAAA);
    bool result = false;
    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.test(1, result));
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.test(15, result));
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.test(0, result));
    TEST_ASSERT_FALSE(result);
}

void test_bitmask_uint32()
{
    BitMask<uint32_t> mask(0xAAAAAAAA);
    bool result = false;
    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.test(1, result));
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.test(31, result));
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.test(0, result));
    TEST_ASSERT_FALSE(result);
}

void test_bitmask_uint64()
{
    BitMask<uint64_t> mask(0xAAAAAAAAAAAAAAAAULL);
    bool result = false;
    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.test(1, result));
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.test(63, result));
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.test(0, result));
    TEST_ASSERT_FALSE(result);
}

void test_bitmask_explicit_conversion()
{
    BitMask<uint8_t> mask(0b10101010);
    uint8_t value = static_cast<uint8_t>(mask);
    TEST_ASSERT_EQUAL(0xAA, value);
}

void test_bitmask_single_uint64_high_bit()
{
    BitMask<uint64_t> mask(1ULL << 63);
    TEST_ASSERT_TRUE(mask.single());
}

void test_bitmask_kmaxbits_uint8()
{
    TEST_ASSERT_EQUAL(8, BitMask<uint8_t>::kActiveBits);
}

void test_bitmask_kmaxbits_uint16()
{
    TEST_ASSERT_EQUAL(16, BitMask<uint16_t>::kActiveBits);
}

void test_bitmask_kmaxbits_uint32()
{
    TEST_ASSERT_EQUAL(32, BitMask<uint32_t>::kActiveBits);
}

void test_bitmask_kmaxbits_uint64()
{
    TEST_ASSERT_EQUAL(64, BitMask<uint64_t>::kActiveBits);
}

void test_bitmask_custom_numbits_4()
{
    // Test with only 4 bits available
    BitMask<uint8_t, 4> mask;

    // Valid positions 0-3 should work
    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.set(0));
    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.set(3));

    // Position 4 and above should fail
    TEST_ASSERT_EQUAL(BitMaskBase::Result::InvalidPosition, mask.set(4));
    TEST_ASSERT_EQUAL(BitMaskBase::Result::InvalidPosition, mask.set(7));

    bool result = false;
    TEST_ASSERT_EQUAL(BitMaskBase::Result::InvalidPosition, mask.test(4, result));
}

void test_bitmask_custom_numbits_1()
{
    // Test with only 1 bit available
    BitMask<uint8_t, 1> mask;

    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.set(0));
    TEST_ASSERT_EQUAL(BitMaskBase::Result::InvalidPosition, mask.set(1));

    bool result = false;
    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.test(0, result));
    TEST_ASSERT_TRUE(result);
}

void test_bitmask_custom_numbits_constructor_masks_value()
{
    // Constructor should mask out bits beyond NumBits
    BitMask<uint8_t, 4> mask(0xFF); // Try to set all 8 bits

    // Only lower 4 bits should be set (0x0F)
    TEST_ASSERT_EQUAL(0x0F, static_cast<uint8_t>(mask));

    bool result = false;
    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.test(0, result));
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL(BitMaskBase::Result::Ok, mask.test(3, result));
    TEST_ASSERT_TRUE(result);
}

void test_bitmask_make_full_mask()
{
    // Test make_full_mask with various sizes
    TEST_ASSERT_EQUAL(0x00, BitMask<uint8_t>::make_full_mask(0));
    TEST_ASSERT_EQUAL(0x01, BitMask<uint8_t>::make_full_mask(1));
    TEST_ASSERT_EQUAL(0x03, BitMask<uint8_t>::make_full_mask(2));
    TEST_ASSERT_EQUAL(0x07, BitMask<uint8_t>::make_full_mask(3));
    TEST_ASSERT_EQUAL(0xFF, BitMask<uint8_t>::make_full_mask(8));

    // When num_bits >= kActiveBits, should return all ones
    TEST_ASSERT_EQUAL(0xFF, BitMask<uint8_t>::make_full_mask(8));
    TEST_ASSERT_EQUAL(0xFF, BitMask<uint8_t>::make_full_mask(10));
}

void test_bitmask_all_with_custom_numbits()
{
    // With 4 bits, all() should check only lower 4 bits
    BitMask<uint8_t, 4> mask(0x0F); // All 4 bits set
    TEST_ASSERT_TRUE(mask.all());

    BitMask<uint8_t, 4> mask2(0xFF); // Upper bits also set, but should still be "all"
    TEST_ASSERT_TRUE(mask2.all());

    BitMask<uint8_t, 4> mask3(0x0E); // Only 3 of 4 bits set
    TEST_ASSERT_FALSE(mask3.all());
}

void test_bitmask_any_with_custom_numbits()
{
    BitMask<uint8_t, 4> mask(0x01);
    TEST_ASSERT_TRUE(mask.any());

    BitMask<uint8_t, 4> mask2(0x10); // Bit 4 is outside our 4-bit range
    TEST_ASSERT_FALSE(mask2.any());

    BitMask<uint8_t, 4> mask3(0x00);
    TEST_ASSERT_FALSE(mask3.any());
}

void test_bitmask_none_with_custom_numbits()
{
    BitMask<uint8_t, 4> mask(0x00);
    TEST_ASSERT_TRUE(mask.none());

    BitMask<uint8_t, 4> mask2(0x10); // Bit 4 is outside our 4-bit range
    TEST_ASSERT_TRUE(mask2.none());

    BitMask<uint8_t, 4> mask3(0x01);
    TEST_ASSERT_FALSE(mask3.none());
}

void test_bitmask_single_with_custom_numbits()
{
    BitMask<uint8_t, 4> mask(0x01);
    TEST_ASSERT_TRUE(mask.single());

    BitMask<uint8_t, 4> mask2(0x08); // Highest valid bit
    TEST_ASSERT_TRUE(mask2.single());

    BitMask<uint8_t, 4> mask3(0x10); // Outside range, should not count
    TEST_ASSERT_FALSE(mask3.single());

    BitMask<uint8_t, 4> mask4(0x03); // Two bits set
    TEST_ASSERT_FALSE(mask4.single());
}

void test_bitmask_reset_with_custom_numbits()
{
    BitMask<uint8_t, 4> mask(0xFF); // Will be masked to 0x0F
    mask.reset();
    TEST_ASSERT_TRUE(mask.none());
    TEST_ASSERT_EQUAL(0x00, static_cast<uint8_t>(mask));
}

void test_bitmask_fill_with_custom_numbits()
{
    BitMask<uint8_t, 4> mask;
    mask.fill(); // Sets all bits to 1 (0xFF)
    // But all() only checks lower 4 bits
    TEST_ASSERT_TRUE(mask.all());
}

void run_test_bitmask_all()
{
    RUN_TEST(test_bitmask_default_constructor);
    RUN_TEST(test_bitmask_explicit_constructor);
    RUN_TEST(test_bitmask_set_ok);
    RUN_TEST(test_bitmask_clear_ok);
    RUN_TEST(test_bitmask_toggle_ok);
    RUN_TEST(test_bitmask_all);
    RUN_TEST(test_bitmask_any);
    RUN_TEST(test_bitmask_none);
    RUN_TEST(test_bitmask_single_zero);
    RUN_TEST(test_bitmask_single_one_bit);
    RUN_TEST(test_bitmask_single_multiple_bits);
    RUN_TEST(test_bitmask_reset);
    RUN_TEST(test_bitmask_fill);
    RUN_TEST(test_bitmask_uint16);
    RUN_TEST(test_bitmask_uint32);
    RUN_TEST(test_bitmask_uint64);
    RUN_TEST(test_bitmask_explicit_conversion);
    RUN_TEST(test_bitmask_single_uint64_high_bit);

    // NumBits template parameter tests
    RUN_TEST(test_bitmask_kmaxbits_uint8);
    RUN_TEST(test_bitmask_kmaxbits_uint16);
    RUN_TEST(test_bitmask_kmaxbits_uint32);
    RUN_TEST(test_bitmask_kmaxbits_uint64);
    RUN_TEST(test_bitmask_custom_numbits_4);
    RUN_TEST(test_bitmask_custom_numbits_1);
    RUN_TEST(test_bitmask_custom_numbits_constructor_masks_value);
    RUN_TEST(test_bitmask_make_full_mask);
    RUN_TEST(test_bitmask_all_with_custom_numbits);
    RUN_TEST(test_bitmask_any_with_custom_numbits);
    RUN_TEST(test_bitmask_none_with_custom_numbits);
    RUN_TEST(test_bitmask_single_with_custom_numbits);
    RUN_TEST(test_bitmask_reset_with_custom_numbits);
    RUN_TEST(test_bitmask_fill_with_custom_numbits);

    // Invalid position tests
    RUN_TEST(test_bitmask_set_invalid_position);
    RUN_TEST(test_bitmask_clear_invalid_position);
    RUN_TEST(test_bitmask_toggle_invalid_position);
    RUN_TEST(test_bitmask_test_invalid_position);
}
