#include "unity_fixture.h"

static void RunAllTests()
{
    RUN_TEST_GROUP(test)
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}