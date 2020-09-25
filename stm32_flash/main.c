#include <stdio.h>
#include <string.h>
#include "binary.h"
#include "port.h"
#include "ymodem.h"
#include "serial.h"
#include <sys/epoll.h>
#include <pthread.h>

void * p_binary = NULL;
struct port_interface *port = NULL;
int transport;

int Epoll_id;
struct epoll_event events[6];


/* settings */
struct port_options port_opts = {
	.device			= "/dev/ttyS2",
	.baudRate		= SERIAL_BAUD_115200,
	.serial_mode		= "8N1",
	.bus_addr		= 0,
	.rx_frame_max		= 256,
	.tx_frame_max		= 256,
};

int EpollInit(int fd)
{
	struct epoll_event event;	
	Epoll_id = epoll_create(1);

	event.data.fd = fd;
	event.events = EPOLLIN | EPOLLET;

	if (epoll_ctl(Epoll_id, EPOLL_CTL_ADD, fd, &event) != 0) {
		printf("mojl epoll add fail \n");
		return -1;
	}

	return 0;
}

void printf_hex(char *buf, int ret)
{
	if (ret <= 0)
		return;
	
	while (ret--)
		printf("%02x", *(buf++));

	printf("\r\n");
}
#if 0

{
	
	ssize_t r;
	uint8_t *pos = (uint8_t *)buf;

	while (count) {
		r = read(mcu_fd, pos, count);
		if (r == 0)
			break;
		if (r < 0)
			break;

		count -= r;
		pos += r;
	}
}

#endif

void receive_command()
{
	int num;
	char buf[2048];
	int nbytes = sizeof(buf);
	int i;
	int ret;

	printf("thread receive command\n");
	
	while (1) {
		num = epoll_wait(Epoll_id, events, 6, -1);

		if (num <= 0) {
			printf("mojl wait no \n");
			continue;
		}

		printf("mojl epoll return num %d\n", num);

		for (i = 0; i < num; i++) {
			memset(buf, 0, sizeof(buf));
			if (events[i].events & EPOLLIN) {
				//ret = port->read(port, buf, nbytes);
				//printf("receive ret %d %s\n", ret, buf);
				printf_hex(buf, ret);
			}
		}
	}
}
void print_hex(const char *buf, int size)
{
	int i = 0;

	printf("str: \n");

	for(i=0; i<size; i++) {
		printf("%x=%02x, ", &(buf[i]), buf[i]);
	}
	printf("\n");
}

int main(int argc, char* argv[])
{

	char * portname = NULL;
	pthread_t rec_command;

	int transport = UART_TRANSPORT;

	int i =0;
	int ret;

	long t = 0;

	//int ret = 0;
	unsigned char buf[256];
	int size = 0;

	int num = 1;
	//int i;
	serial_t *h;

	for (i = 0; i < 10; i++)
		buf[i] = i;

	printf_hex(buf, 5);
	
	print_hex(buf, 5);
	//return 0;

#if 0	
	if (argc < 2) {
		//printf_help();
		//exit(1);
	}

	for (i = 1; i < argc; i++) {
        /* skip bad arguments */
        if (argv[i] == NULL || *argv[i] == 0 || *argv[i] != '-') {
            continue;
        }		
		if (strcmp(argv[i], "-h") == 0) {
			printf_help();
			return 0;
		}
        if (strcmp(argv[i], "-p") == 0) {
			if (i + 1 >= argc) {
                printf("No parameter specified with -p, aborting.\n");
                exit(1);
            }
            portname = strdup(argv[i + 1]);
         }		
	}

	//printf("transport %d bl_mode %d\n", transport, bl_mode);  


	if (portname != NULL)
		port_opts.device = portname;
#endif
	printf("device name %s\n", port_opts.device);

	if (port_open(&port_opts, &port, transport) != PORT_ERR_OK) {
		printf("Failed to open port: %s\n", port_opts.device);
		//exit(1);
	}

#if 1
	h = (serial_t *)port->private;
	EpollInit(h->fd);

	while (1) {
		//num = epoll_wait(Epoll_id, events, 6, -1);

		if (num <= 0) {
			printf("mojl wait no \n");
			return -1;
		}

		printf("mojl epoll return num %d\n", num);

		for (i = 0; i < num; i++) {
			memset(buf, 0, sizeof(buf));
			if (1) {//events[i].events & EPOLLIN) {
				ret = port->read(port, (void *)buf, sizeof(buf), (size_t *)&size);
				//printf("receive ret %d %s\n", ret, buf);
				printf_hex(buf, size);
				print_hex(buf, size);
			}
		}
	}

#else
	pthread_create(&rec_command, NULL, (void *)rec_command_loop, (void *)port);
	pthread_join(rec_command, NULL);
#endif

	printf("main will exit\n");



	port->close(port);

	if (portname != NULL) {
		//free(portname);
		portname = NULL;
	}	

	return 0;
}

	
