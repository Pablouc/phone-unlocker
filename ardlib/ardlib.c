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

int lock_device(ardlib_ctx_t *ctx) {
  sem_wait(&ctx->driver_sem);

  int fd = open("/dev/arduino_spi_device", O_RDWR);
  if (fd < 0) {
    printf("Failed to open driver file, driver could be missing\n");
  }

  return fd;
}

void unlock_device(ardlib_ctx_t *ctx, int fd) {
  close(fd);
  sem_post(&ctx->driver_sem);
}

int write_to_device(int fd, void *data) {
  write(fd, data, 1);
  char read_value;
  if ( read(fd, &read_value, 1) < 0 ) {
    printf("There was an error reading the response of the device!\n");
    return 1;
  }

  if (read_value != 10) {
    printf("Device reported an error!\n");
    return 1;
  } 
  printf("Successfully talked to device\n");
  return 0;
}

int press_button(ardlib_ctx_t *ctx, int button){

  if (button < 0 || button > 9) {
    printf("Button sizes not expected!\n");
    return 1;
  }

  int fd = lock_device(ctx);
  if (write_to_device(fd, &button) < 0) {
    unlock_device(ctx, fd);
    return 1;
  }

  unlock_device(ctx, fd);
  return 0;
}

int change_size(ardlib_ctx_t *ctx, int size) {
  int data_to_write = 0;
  switch(size) {
    case 1:
      data_to_write = 11;
      break;
    case 2:
      data_to_write = 22;
      break;
    case 3:
      data_to_write = 33;
      break;
    default:
      printf("Screen size not expected\n");
      return 1;
      break;
  }

  int fd = lock_device(ctx);
  if (write_to_device(fd, &data_to_write) < 0) {
    unlock_device(ctx, fd);
    return 1;
  }

  unlock_device(ctx, fd);
  return 0;
}
