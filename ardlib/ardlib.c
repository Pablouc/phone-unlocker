#include "ardlib.h"
#include <semaphore.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int init_ardlib(ardlib_ctx_t *context) {
  FILE *file;
  file = fopen("/dev/arduino_spi_device", "r");
  if (file == NULL) {
    printf("Failed to open driver file, driver could be missing\n");
    return 1;
  }

  if (sem_init(&context->driver_sem, 0, 1) < 0){
    printf("Failed to initialize semaphore!\n");
    return 1;
  }
  fclose(file);
  return 0;
}

int press_button(ardlib_ctx_t *ctx, int button){
  sem_wait(&ctx->driver_sem);

  int fd = open("/dev/arduino_spi_device", O_RDWR);
  if (fd < 0) {
    printf("Failed to open driver file, driver could be missing\n");
    return 1;
  }

  write(fd, &button, 1);
  char read_value;
  if ( read(fd, &read_value, 1) < 0 ) {
    printf("There was an error reading the response of the device!\n");
    close(fd);
    return 1;
  }

  if (read_value != 10) {
    printf("Device reported an error!\n");
    close(fd);
    return 1;
  } 
  printf("Successfully talked to device\n");

  close(fd);
  sem_post(&ctx->driver_sem);
  return 0;
}
