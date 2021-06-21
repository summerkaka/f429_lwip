#include "lwip.h"
#include "lwip/ip.h"
#include "lwip/dhcp.h"
#include "lwip_demo.h"

#define MAX_DHCP_TRIES  4

volatile dhcp_state_t DHCP_state = DHCP_OFF;
extern u32_t DHCPfineTimer; // for lwip.c already defined it


/**
  * @brief  DHCP_Process_Handle
  * @param  None
  * @retval None
  */
static void DHCP_Process(struct netif *netif)
{
    ip_addr_t ipaddr;
    ip_addr_t netmask;
    ip_addr_t gw;
    struct dhcp *dhcp;

    switch (DHCP_state) {
    case DHCP_OFF: {
        DHCP_state = netif_is_up(netif) ? DHCP_START : DHCP_OFF;
    } break;
    case DHCP_START: {
        ip_addr_set_zero_ip4(&netif->ip_addr);
        ip_addr_set_zero_ip4(&netif->netmask);
        ip_addr_set_zero_ip4(&netif->gw);
        DHCP_state = DHCP_WAIT_ADDRESS;
        dhcp_start(netif);
    } break;
    case DHCP_WAIT_ADDRESS: {
        if (dhcp_supplied_address(netif)) {
            DHCP_state = DHCP_ADDRESS_ASSIGNED;
            u32_t ip_addr = netif->ip_addr.addr;
            printf("get ip address: %d.%u.%lu.%lu\n", (unsigned int)(ip_addr >> 24 & 0xff), (unsigned int)(ip_addr >> 16 & 0xff), ip_addr >> 8 & 0xff, ip_addr & 0xff);
            HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
            // BSP_LED_On(LED1);
        } else {
            dhcp = (struct dhcp *)netif_get_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);

            /* DHCP timeout */
            if (dhcp->tries > MAX_DHCP_TRIES) {
                DHCP_state = DHCP_TIMEOUT;

                /* Stop DHCP */
                dhcp_stop(netif);

                /* Static address used */
                IP_ADDR4(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
                IP_ADDR4(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
                IP_ADDR4(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
                netif_set_addr(netif, &ipaddr, &netmask, &gw);
                // BSP_LED_On(LED1);
            }
        }
    } break;
    case DHCP_LINK_DOWN: {
        /* Stop DHCP */
        dhcp_stop(netif);
        DHCP_state = DHCP_OFF;
        HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
    } break;
    default:
        break;
    }
}

/**
  * @brief  DHCP periodic check
  * @param  localtime the current LocalTime value
  * @retval None
  */
void DHCP_Periodic_Handle(struct netif *netif)
{
    /* Fine DHCP periodic process every 500ms */
    if (HAL_GetTick() - DHCPfineTimer >= DHCP_FINE_TIMER_MSECS) {
        DHCPfineTimer = HAL_GetTick();
        /* process DHCP state machine */
        DHCP_Process(netif);
        // print_dhcp(netif);
    }
}

void print_dhcp(struct netif *nif)
{
    u32_t ip_addr = nif->ip_addr.addr;

    printf("dhcp: %d, ip: %d.%u.%lu.%lu\n", DHCP_state, (unsigned int)(ip_addr >> 24 & 0xff), (unsigned int)(ip_addr >> 16 & 0xff), ip_addr >> 8 & 0xff, ip_addr & 0xff);
}
