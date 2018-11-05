#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>

#include <i3ds/time.hpp>
#include <i3ds/imu_sensor.hpp>
#include <i3ds/communication.hpp>
#include <i3ds/publisher.hpp>

#define BUFFER_SIZE 4096

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
	int more = 1;

	bzero(buffer, BUFFER_SIZE);

	while (more) {

		//printf("Sending address %X\r\n", address);

		bytes_sent = write(fd, &address, sizeof(uint32_t));

		if (bytes_sent != sizeof(uint32_t)) {
			perror("Failed to write data!");
			return -1;
		}

		//printf("Reading address %X\r\n", address);

		bytes_read = read(fd, payload, PAYLOAD_SIZE);

		if (bytes_read != PAYLOAD_SIZE) {
			perror("Failed to read data");
			return -1;
		}

		//printf("Got %d bytes\r\n", bytes_read);

		if (payload->total > BUFFER_SIZE) {
			printf("Insufficient buffer size");
			return -1;
		}

		if (payload->offset + payload->size > payload->total) {
			printf("Payload size vs total mismatch");
			return -1;
		}

		memcpy(buffer + payload->offset, payload->data, payload->size);
		more = payload->size + payload->offset < payload->total;
	}

	return payload->offset;
}

int main(int argc, char *argv[])
{
	int running = 1;
	int opt;
	int size;

	std::string rpmsg_dev = "/dev/rpmsg0";

	i3ds::Context::Ptr context = i3ds::Context::Create();

	// TODO: make dynamic and nice
	NodeID node = 310;

	i3ds::Publisher publisher(context, node);

	printf("\r\n I3DS R5 IMU bridge \r\n");
	printf("\r\n Open rpmsg dev! \r\n");

	fd = open(rpmsg_dev.c_str(), O_RDWR);

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

	uint32_t address_id = (node << 8) + i3ds::IMU::MeasurementTopic::endpoint;

	while (running) {

		IMUMeasurement20 measurement;

		size = remote_read(address_id);

		if (size < 0 || (size % sizeof(IMUSample))) {
			printf("Error reading %d", size);
			break;
		}

		if (size == 0) {
		  continue;
		}

		const int n = size / sizeof(IMUSample);

		printf("Got %d samples", n);

		IMUMeasurement20_Initialize(&measurement);

		measurement.attributes.timestamp = i3ds::get_timestamp();
		measurement.attributes.validity = sample_valid;

		measurement.batch_size = n;
		measurement.samples.nCount = n;

		memcpy(measurement.samples.arr, buffer, size);

		publisher.Send<i3ds::IMU::MeasurementTopic>(measurement);
	}

	close(fd);

	printf("\r\n Quitting application .. \r\n");
	printf(" I3DS R5 IMU bridge end \r\n");

	free(payload);
	free(buffer);

	return 0;
}
