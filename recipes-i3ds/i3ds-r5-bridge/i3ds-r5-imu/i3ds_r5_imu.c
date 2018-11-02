#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_SIZE 4000

#define RPMSG_GET_KFIFO_SIZE 1
#define RPMSG_GET_AVAIL_DATA_SIZE 2
#define RPMSG_GET_FREE_SPACE 3

#define RPMSG_HEADER_LEN 16
#define MAX_RPMSG_BUFF_SIZE (512 - RPMSG_HEADER_LEN)

#define PAYLOAD_SIZE (MAX_RPMSG_BUFF_SIZE)
#define HEADER_SIZE  16
#define DATA_SIZE    (PAYLOAD_SIZE - HEADER_SIZE)

typedef struct _payload {	
	uint32_t address;
	uint32_t offset;
	uint32_t total;	
	uint32_t size;
	uint8_t  data[DATA_SIZE];
} payload_t;

static int fd;

static uint8_t *buffer;
static payload_t *payload;

int remote_read(uint32_t address)
{
	int bytes_sent, bytes_read;
	int offset = 0;
	int more = 1;

	bzero(buffer, BUFFER_SIZE);		

	while (more) {

		printf("Sending address %X\r\n", address);
	  
		bytes_sent = write(fd, &address, sizeof(uint32_t));

		if (bytes_sent != sizeof(uint32_t)) {
			perror("Failed to write data!");
			return -1;
		}

		printf("Reading address %X\r\n", address);

		bytes_read = read(fd, payload, PAYLOAD_SIZE); 

		if (bytes_read != PAYLOAD_SIZE) {
			perror("Failed to read data");
			return -1;
		}

		printf("Got %d bytes\r\n", bytes_read);

		if (payload->total > BUFFER_SIZE) {
			printf("Insufficient buffer size");
			return -1;
		}

		if (offset + payload->size > payload->total) {
			printf("Payload size vs total mismatch");
			return -1;
		}

		printf("Payload offset %d, %d bytes total, size %d bytes\r\n",
		       payload->offset,
		       payload->total,
		       payload->size);
		
		for (int i = 0; i < payload->size; i++) {
			buffer[offset + i] = payload->data[i];
		}

		offset += payload->size;
		more = offset < payload->total;
	}	

	return offset;
}

int main(int argc, char *argv[])
{
	int running = 1;
	int opt;
	int size;

	char *rpmsg_dev = "/dev/rpmsg0";

	// TODO: Add meaningful address.
	uint32_t address = 400;
	
	while ((opt = getopt(argc, argv, "d:")) != -1) {
		switch (opt) {
		case 'd':
			rpmsg_dev = optarg;
			break;
		default:
			printf("getopt return unsupported option: -%c\n",opt);
			break;
		}
	}
	
	printf("\r\n I3DS R5 IMU bridge \r\n");
	printf("\r\n Open rpmsg dev! \r\n");

	fd = open(rpmsg_dev, O_RDWR);

	if (fd < 0) {
		perror("Failed to open rpmsg file /dev/rpmsg0.");
		return -1;
	}

	payload = (payload_t *) malloc(sizeof(payload_t));

	if (!payload) {
		perror("Failed allocate payload.");
		return -1;
	}

	buffer = (uint8_t *) malloc(BUFFER_SIZE);

	if (!buffer) {
		perror("Failed allocate buffer.");
		return -1;
	}

	while (running) {
		size = remote_read(address);

		if (size < 0) {
			break;
		}

		printf(" read address %X, received %d bytes", address, size);
	}

	close(fd);
	
	printf("\r\n Quitting application .. \r\n");
	printf(" I3DS R5 IMU bridge end \r\n");

	free(payload);
	free(buffer);

	return 0;
}
