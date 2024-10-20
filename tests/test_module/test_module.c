#include "unity_fixture.h"
#include "fff.h"

TEST_GROUP(test);

TEST_SETUP(test)
{
    // Insert setup procedure
};

TEST_TEAR_DOWN(test)
{
    // Insert tear down procedure
};

TEST(test, fail_test)
{
    TEST_FAIL();
};

TEST(test, success_test)
{
    TEST_ASSERT_EQUAL(1, 1);
};