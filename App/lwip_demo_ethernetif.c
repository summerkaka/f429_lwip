#include "ethernetif.h"
#include "lwip_demo.h"

void ethernetif_notify_conn_changed(struct netif *netif)
{
    /* NOTE : This is function could be implemented in user file
            when the callback is needed, */
    if(netif_is_link_up(netif)) {
        netif_set_up(netif);
    } else {
        netif_set_down(netif);
        DHCP_state = DHCP_LINK_DOWN;
    }
}
