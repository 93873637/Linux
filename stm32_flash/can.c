#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <time.h>
#include "port.h"

struct can_priv {
	int sck;
	char device[12];
};

long getTickCount()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (long)(ts.tv_nsec/1000) + (long)(ts.tv_sec * 1000000);
}

static port_err_t can_open(struct port_interface *port,
				    struct port_options *ops)
{
	struct can_priv *h;
	int sck;
	struct sockaddr_can addr;
	struct ifreq ifq;
	struct can_filter rfilter;

	h = calloc(sizeof(struct can_priv), 1);
	if (h == NULL) {
		fprintf(stderr, "End of memory\n");
		return PORT_ERR_UNKNOWN;
	}

	sck = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if (ops->device != NULL) {
		strcpy(ifq.ifr_name, ops->device);
	} else {
		strcpy(ifq.ifr_name, "can3");
	}

	ioctl(sck, SIOCGIFINDEX, &ifq);

	addr.can_family = AF_CAN;
	addr.can_ifindex = ifq.ifr_ifindex;

	bind(sck, (struct sockaddr *)&addr, sizeof(addr));

	rfilter.can_id = 0x11;
	rfilter.can_mask = CAN_SFF_MASK;

	setsockopt(sck, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

	h->sck = sck;
	strcpy(h->device, ifq.ifr_name);

	port->private = (void *)h;

	return PORT_ERR_OK;
}

static port_err_t can_close(struct port_interface *port)
{
	struct can_priv *h;

	h = (struct can_priv *)port->private;
	if (h == NULL)
		return PORT_ERR_UNKNOWN;
	close(h->sck);
	free(h);
	port->private = NULL;
	return PORT_ERR_OK;
}

static port_err_t can_read(struct port_interface *port, void *buf,
			   size_t nbyte)
{
	int len;
	struct can_frame frame;
	struct can_priv *h;
	uint8_t *pos = (uint8_t *)buf;
	h = (struct can_priv *)port->private;
	if (h == NULL)
		return PORT_ERR_UNKNOWN;

	while (nbyte) {
		len = read(h->sck, &frame, sizeof(frame));
		if (len > 0) {
			//printf("ID=0x%x DLC=%d\n", frame.can_id, frame.can_dlc);
			nbyte -= frame.can_dlc;
			memcpy(pos, frame.data, frame.can_dlc);
			//printf("can read 0x%x\n", *pos);
			pos += frame.can_dlc;
			
		} else {
			printf("read NULL or fail\n");
			return PORT_ERR_TIMEDOUT;
		}
	}

	return PORT_ERR_OK;
		
}

static port_err_t can_write(struct port_interface *port, void *buf,
			    size_t nbyte)
{
	int len;
	struct can_frame frame;
	struct can_priv *h;
	
	h = (struct can_priv *)port->private;
	if (h == NULL)
		return PORT_ERR_UNKNOWN;
	
	if (nbyte > CAN_MAX_DLEN)
		return PORT_ERR_OK;
	frame.can_id = 0x11;
	frame.can_dlc = nbyte;
	memcpy(frame.data, buf, nbyte);
	
	//long t = getTickCount();
	usleep(2000);

	//printf("can write nbytes %d  0x%x\n", nbyte, frame.data[0]);
	//printf("t %ld us\n", getTickCount());
	while (1) {
		len = write(h->sck, &frame, sizeof(frame));
		if (len == sizeof(frame)) {
			//printf("can write success len %d\n", len);
			break;
		} else {
			printf("can write len %d fail \n", len);
		}
	}

	return PORT_ERR_OK;
}

static port_err_t can_gpio(struct port_interface *port, serial_gpio_t n,
			   int level)
{
	return PORT_ERR_OK;
}

static const char *can_get_cfg_str(struct port_interface *port)
{
	struct can_priv *h;
	static char str[12];

	h = (struct can_priv *)port->private;
	if (h == NULL)
		return "INVALID";
	snprintf(str, sizeof(str), "%s", h->device);
	return str;
}

struct port_interface port_can = {
	.name	= "can",
	.flags	= PORT_STRETCH_W,
	.open	= can_open,
	.close	= can_close,
	.read	= can_read,
	.write	= can_write,
	.gpio	= can_gpio,
	.get_cfg_str	= can_get_cfg_str,
};
