///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include "remoteproc_bridge.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>

#define RPMSG_GET_KFIFO_SIZE 1
#define RPMSG_GET_AVAIL_DATA_SIZE 2
#define RPMSG_GET_FREE_SPACE 3

#define RPMSG_HEADER_LEN 16
#define MAX_RPMSG_BUFF_SIZE (512 - RPMSG_HEADER_LEN)

#define PAYLOAD_SIZE (MAX_RPMSG_BUFF_SIZE)
#define HEADER_SIZE  16
#define DATA_SIZE    (PAYLOAD_SIZE - HEADER_SIZE)

typedef struct _payload
{
  uint32_t address;
  uint32_t offset;
  uint32_t total;
  uint32_t size;
  uint8_t  data[DATA_SIZE];
} payload_t;

static int fd;
static payload_t *payload;

int
i3ds::remoteproc_open(std::string dev)
{
  fd = open(dev.c_str(), O_RDWR);

  if (fd < 0)
    {
      perror("Failed to open rpmsg file /dev/rpmsg0.");
      return -1;
    }

  payload = (payload_t *) malloc(sizeof(payload_t));

  if (!payload)
    {
      perror("Failed allocate payload.");
      return -1;
    }

  return 0;
}

int
i3ds::remoteproc_read(uint32_t address, uint8_t *buffer, size_t size)
{
  int bytes_sent, bytes_read;
  int more = 1;

  while (more)
    {

      bytes_sent = write(fd, &address, sizeof(uint32_t));
      
      if (bytes_sent != sizeof(uint32_t))
	{
	  perror("Failed to write data!");
	  return -1;
	}

      bytes_read = read(fd, payload, PAYLOAD_SIZE);

      if (bytes_read != PAYLOAD_SIZE)
	{
	  perror("Failed to read data");
	  return -1;
	}

      if (payload->total > size)
	{
	  printf("Insufficient buffer size");
	  return -1;
	}

      if (payload->offset + payload->size > payload->total)
	{
	  printf("Payload size vs total mismatch");
	  return -1;
	}

      memcpy(buffer + payload->offset, payload->data, payload->size);

      more = payload->offset + payload->size < payload->total;
    }

  return payload->total;
}

int
i3ds::remoteproc_close()
{
  close(fd);
  free(payload);

  fd = 0;
  payload = NULL;

  return 0;
}
