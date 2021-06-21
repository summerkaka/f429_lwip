#ifndef __LWIP_DEMO_H__
#define __LWIP_DEMO_H__


#define DEST_IP_ADDR0   (uint8_t)192
#define DEST_IP_ADDR1   (uint8_t)168
#define DEST_IP_ADDR2   (uint8_t)0
#define DEST_IP_ADDR3   (uint8_t)11

#define DEST_PORT       (uint16_t)7

/*Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
#define IP_ADDR0        (uint8_t)192
#define IP_ADDR1        (uint8_t)168
#define IP_ADDR2        (uint8_t)0
#define IP_ADDR3        (uint8_t)10

/*NETMASK*/
#define NETMASK_ADDR0   (uint8_t)255
#define NETMASK_ADDR1   (uint8_t)255
#define NETMASK_ADDR2   (uint8_t)255
#define NETMASK_ADDR3   (uint8_t)0

/*Gateway Address*/
#define GW_ADDR0        (uint8_t)192
#define GW_ADDR1        (uint8_t)168
#define GW_ADDR2        (uint8_t)0
#define GW_ADDR3        (uint8_t)1



typedef enum {
    DHCP_OFF,
    DHCP_START,
    DHCP_WAIT_ADDRESS,
    DHCP_ADDRESS_ASSIGNED,
    DHCP_TIMEOUT,
    DHCP_LINK_DOWN
} dhcp_state_t;

extern volatile dhcp_state_t DHCP_state;


void echo_client_init(void);
void http_server_init(void);
void DHCP_Periodic_Handle(struct netif *netif);
void print_dhcp(struct netif *nif);


#endif
