add_test([=[Simple.Works]=]  [==[C:/Users/letha/Documents/UTS/Advanced Algorithms/assessment 3/Assessment-3-advance-al/build/Debug/AssessmentTests.exe]==] [==[--gtest_filter=Simple.Works]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[Simple.Works]=]  PROPERTIES WORKING_DIRECTORY [==[C:/Users/letha/Documents/UTS/Advanced Algorithms/assessment 3/Assessment-3-advance-al/build]==] SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  AssessmentTests_TESTS Simple.Works)
