#include <unity.h>

void run_test_bitmask_all();
void run_test_bitbucket();

void setUp() {}
void tearDown() {}

int main()
{
    UNITY_BEGIN();

    run_test_bitmask_all();
    run_test_bitbucket();

    return UNITY_END();
}
