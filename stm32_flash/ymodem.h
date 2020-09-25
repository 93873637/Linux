#include <stdint.h>
#include <string.h>

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/


#define PACKET_HEADER           0xee
#define POWER_OFF               0x01
#define REBOOT                  0x02
#define GET_TEMPERATURE         0x03


static void XCU_PreparePacket(uint8_t *packet, uint8_t *data, uint8_t size);
static int32_t XCU_Parse_Packet (uint8_t *packet, uint8_t *data, uint8_t *length);
int handle_rec_command(struct port_interface *port, uint8_t *buf, int size);
