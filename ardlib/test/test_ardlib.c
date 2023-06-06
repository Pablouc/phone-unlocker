#include "ardlib.h"
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

ardlib_ctx_t *ctx;

int expect_equal(int expected, int actual) {
  if (expected != actual) {
    printf("Expected %d, got %d\n", expected, actual);
    return 1;
  }
  printf("Ok.\n");
  return 0;
}

int test_lib_should_initialize() {
  ardlib_ctx_t *ctx = malloc(sizeof(ardlib_ctx_t));
  int result = init_ardlib(ctx);
  free(ctx);
  return expect_equal(0, result);
}

int test_press_button() {
  printf("Testing press_button...\n");
  int result = press_button(ctx, 1);
  return expect_equal(0, result);
}

void setup() {
  ctx = malloc(sizeof(ardlib_ctx_t));
  init_ardlib(ctx);
}

int main() {
  printf("Starting test_ardlib tests...\n");
  int result = 0;

  setup();

  result += test_lib_should_initialize();
  result += test_press_button();

  if (result == 0) {
    printf("All tests completed succesfully!\n");
  }
  return result;
}
