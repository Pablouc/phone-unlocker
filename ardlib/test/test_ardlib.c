#include "ardlib.h"
#include <stdio.h>

int expect_equal(int expected, int actual) {
  if (expected != actual) {
    printf("Expected %d, got %d\n", expected, actual);
    return 1;
  }
  printf("Ok.\n");
  return 0;
}

int test_press_button() {
  printf("Testing press_button...\n");
  int result = press_button(1);
  return expect_equal(1, result);
}

int main() {
  printf("Starting test_ardlib tests...\n");
  int result = 0;

  result += test_press_button();

  if (result == 0) {
    printf("All tests completed succesfully!\n");
  }
  return result;
}
