/**
  ******************************************************************************
  * @file    IAP/src/ymodem.c 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    10/15/2010
  * @brief   This file provides all the software functions related to the ymodem 
  *          protocol.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/** @addtogroup IAP
  * @{
  */ 
  
/* Includes ------------------------------------------------------------------*/
//#include "common.h"
//#include "stm32f10x_flash.h"
#include "serial.h"
#include "binary.h"
#include "port.h"
#include "ymodem.h"
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Update CRC16 for input byte
  * @param  CRC input value 
  * @param  input byte
   * @retval None
  */
uint16_t UpdateCRC16(uint16_t crcIn, uint8_t byte)
{
	uint32_t crc = crcIn;
	uint32_t in = byte|0x100;
	do {
		crc <<= 1;
		in <<= 1;
		if(in&0x100)
			++crc;
		if(crc&0x10000)
			crc ^= 0x1021;
	} while(!(in&0x10000));
	
	return crc&0xffffu;
}


/**
  * @brief  Cal CRC16 for YModem Packet
  * @param  data
  * @param  length
   * @retval None
  */
uint16_t Cal_CRC16(const uint8_t* data, uint32_t size)
{
	uint32_t crc = 0;
	const uint8_t* dataEnd = data+size;
	while(data<dataEnd)
		crc = UpdateCRC16(crc,*data++);
 
	crc = UpdateCRC16(crc,0);
	crc = UpdateCRC16(crc,0);
	return crc&0xffffu;
}

/**
  * @brief  Cal Check sum for YModem Packet
  * @param  data
  * @param  length
   * @retval None
  */
uint8_t CalChecksum(const uint8_t* data, uint32_t size)
{
	uint32_t sum = 0;
	const uint8_t* dataEnd = data+size;
	while(data < dataEnd )
		sum += *data++;
	return sum&0xffu;
}

/**
  * @brief  Receive a packet from sender
  * @param  data
  * @param  length
  * @param  timeout
  *     0: end of transmission
  *    -1: abort by sender
  *    >0: packet length
  * @retval 0: normally return
  *        -1: timeout or packet error
  *         1: abort by user
  */
static int32_t XCU_Parse_Packet (uint8_t *packet, uint8_t *data, uint8_t *length)
{
	uint8_t data_len = 0;
	if ((packet==NULL) || (data==NULL) || (length==NULL)) {
		printf("receive packet input params fail\n");
		return -1;
	}
	
	if (packet[0] != PACKET_HEADER) {
		printf("receive packet header fail\n");
		return -1;
	}
	
	data_len = packet[1];
	if (data_len <= 0) {
		printf("receive packet payload is 0\n");
		return -1;
	}
	memcpy(data, packet+2, data_len);

	if (packet[data_len+2] == CalChecksum(packet, data_len+2)) {
		*length = data_len;
	} else {
		printf("receive packet crc fail\n");
		return -1;
	}
		

	return 0;
}






/**
  * @brief  Prepare the data packet
  * @param  timeout
  *     0: end of transmission
  */
static void XCU_PreparePacket(uint8_t *packet, uint8_t *data, uint8_t size)
{
	if ((packet==NULL) || (data==NULL) || (size==0)) {
		printf("prepare packet input params have NULL\n");
		return;
	}
	packet[0] = PACKET_HEADER;
	packet[1] = size;
	memcpy(packet+2, data, size);
	packet[size+2] = CalChecksum(data, size+2);	
}

static void XCU_power_off()
{
	printf("XCU power off\n");
}

static void XCU_reboot()
{
	printf("XCU reboot\n");
}

static void XCU_get_temperature()
{
	printf("XCU get temperature\n");
}

int handle_rec_command(struct port_interface *port, uint8_t *buf, int size)
{
	uint8_t payload[8];
	uint8_t len = 0;
	uint8_t command = 0;

	if ((XCU_Parse_Packet (buf, payload, &len)) < 0 || len) {
		printf("handle command packet fail\n");
		return -1;
	}

	command = payload[0];
	switch (command) {
	case POWER_OFF:
		XCU_power_off();
		break;
	case REBOOT:
		XCU_reboot();
		break;
	case GET_TEMPERATURE:
		XCU_get_temperature();
		break;

	default:
		printf("handle command unsupported \n");
		break;
	}

	return 0;	
}
