#include "lwip/init.h"
#include "lwip/ip.h"
#include "lwip/netif.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"
//#include "lwip/timers.h"
#include "lwip/udp.h"
#include "netif/etharp.h"
#include <stdio.h>

void http_server_init(void);


#define HTTP_PORT 80
const unsigned char htmldata[] = "	\
        <html>	\
        <head><title> A LwIP WebServer !!</title></head> \
	    <center><p>A WebServer Based on LwIP v1.4.1!</center>\
	    </html>";

/**
  * @brief code for a http server, send HTTP data to any connected client, and then close connection
  */
static err_t http_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
    char *data = NULL;
    /* We perform here any necessary processing on the pbuf */
    if (p != NULL) {
        /* We call this function to tell the LwIp that we have processed the data */
        /* This lets the stack advertise a larger window, so more data can be received*/
        tcp_recved(pcb, p->tot_len);

        data = p->payload;
        if (p->len >= 3 && data[0] == 'G' && data[1] == 'E' && data[2] == 'T') {
            tcp_write(pcb, htmldata, sizeof(htmldata), 1);
        } else {
            printf("Request error\n");
        }
        pbuf_free(p);
        tcp_close(pcb);
    } else if (err == ERR_OK) {
        /* When the pbuf is NULL and the err is ERR_OK, the remote end is closing the connection. */
        /* We free the allocated memory and we close the connection */
        return tcp_close(pcb);
    }
    return ERR_OK;
}

/**
  * @brief  This function when the Telnet connection is established
  * @param  arg  user supplied argument
  * @param  pcb	 the tcp_pcb which accepted the connection
  * @param  err	 error value
  * @retval ERR_OK
  */
static err_t http_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{
    tcp_recv(pcb, http_recv);
    return ERR_OK;
}

/**
  * @brief  Initialize the http application
  * @param  None
  * @retval None
  */
void http_server_init(void)
{
    struct tcp_pcb *pcb = NULL;

    /* Create a new TCP control block  */
    pcb = tcp_new();

    /* Assign to the new pcb a local IP address and a port number */
    /* Using IP_ADDR_ANY allow the pcb to be used by any local interface */
    tcp_bind(pcb, IP_ADDR_ANY, HTTP_PORT);

    /* Set the connection to the LISTEN state */
    pcb = tcp_listen(pcb);

    /* Specify the function to be called when a connection is established */
    tcp_accept(pcb, http_accept);
}

#if 0
void lwip_demo(void *pdata)
{
    //init LwIP
    lwip_init_task();

    //setup echo server
    echo_client_init();
    http_server_init();
    //for periodic handle
    while (1) {
        process_mac();

        //process LwIP timeout
        sys_check_timeouts();

        //todo: add your own user code here
    }
}
#endif