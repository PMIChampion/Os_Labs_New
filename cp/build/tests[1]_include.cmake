if(EXISTS "/Users/vladislavburdinskij/Documents/repair_git/OS_labs/cp/build/tests[1]_tests.cmake")
  include("/Users/vladislavburdinskij/Documents/repair_git/OS_labs/cp/build/tests[1]_tests.cmake")
else()
  add_test(tests_NOT_BUILT tests_NOT_BUILT)
endif()
