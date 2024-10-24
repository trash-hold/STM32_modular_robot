#include "unity_fixture.h"

TEST_GROUP_RUNNER(test)
{
    RUN_TEST_CASE(test, fail_test);
    RUN_TEST_CASE(test, success_test);
};

static void RunAllTests()
{
    RUN_TEST_GROUP(test)
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}