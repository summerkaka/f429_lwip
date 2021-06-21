#include "lwip/init.h"
#include "lwip/ip.h"
#include "lwip/netif.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"
//#include "lwip/timers.h"
#include "lwip/udp.h"
#include "netif/etharp.h"
#include <stdio.h>

//extern functions
// extern err_t ethernetif_init(struct netif *netif);
// extern void process_mac(void);
void echo_client_init(void);

//global data
#if 0
struct netif lan8742_netif;

void lwip_init_task(void)
{
    struct ip_addr ipaddr, netmask, gw;

    lwip_init();
    IP4_ADDR(&gw, 192, 168, 1, 1);
    IP4_ADDR(&ipaddr, 192, 168, 1, 37);
    IP4_ADDR(&netmask, 255, 255, 255, 0);

    netif_add(&lan8742_netif, &ipaddr, &netmask, &gw, NULL, ethernetif_init, ethernet_input);
    netif_set_default(&lan8742_netif);
    netif_set_up(&lan8742_netif);
}
#endif

/**
 * @brief code for a echo client
 * client will connect to server first,then send anything received out to server
 * client will never close connection actively
 */
static void echo_client_conn_err(void *arg, err_t err)
{
    printf("connect error! closed by core!!\n");
    printf("try to connect to server again!!\n");
    echo_client_init();
}

static err_t echo_client_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
    /* We perform here any necessary processing on the pbuf */
    if (p != NULL) {
        /* We call this function to tell the LwIp that we have processed the data */
        /* This lets the stack advertise a larger window, so more data can be received*/
        tcp_recved(pcb, p->tot_len);
        tcp_write(pcb, p->payload, p->len, 1);
        pbuf_free(p);

    } else if (err == ERR_OK) {
        /* When the pbuf is NULL and the err is ERR_OK, the remote end is closing the connection. */
        /* We free the allocated memory and we close the connection */
        tcp_close(pcb);
        echo_client_init();
        return ERR_OK;
    }
    return ERR_OK;
}

static err_t echo_client_sent(void *arg, struct tcp_pcb *pcb, u16_t len)
{
    printf("echo client send data OK! sent len = [%d]\n", len);
    return ERR_OK;
}

static err_t echo_client_connected(void *arg, struct tcp_pcb *pcb, err_t err)
{
    char GREETING[] = "Hi, I am a new Client!\n";

    tcp_recv(pcb, echo_client_recv);
    tcp_sent(pcb, echo_client_sent);

    tcp_write(pcb, GREETING, sizeof(GREETING), 1);
    return ERR_OK;
}

void echo_client_init(void)
{
    struct tcp_pcb *pcb = NULL;
    ip4_addr_t server_ip;
    /* Create a new TCP control block  */
    pcb = tcp_new();
    IP4_ADDR(&server_ip, 192, 168, 1, 11);
    tcp_connect(pcb, &server_ip, 21, echo_client_connected);
    tcp_err(pcb, echo_client_conn_err);
}




