/**
  ******************************************************************************
  * @file    Project/src/usart_printf.c
  * @author   xiatian
  * @version V0.00
  * @date
  * @brief   console.c
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "usart.h"

/* Private macro -------------------------------------------------------------*/
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(FILE *f)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif

/* Private typedef -----------------------------------------------------------*/
#define UARTHANDLE &huart6
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Code begin ----------------------------------------------------------------*/
// FILE uart_terminal = {
//     ._read = _read,
//     ._write = _write
// };

// static int _read(struct _reent *r, void *p, char *ptr, int len)
// {
//     int i = 0;
//     uint8_t ch;
//     while (i++ < len) {
//         if (HAL_UART_Receive(UARTHANDLE, &ch, 1, HAL_MAX_DELAY) != HAL_OK)
//             break;
//         *ptr++ = ch;
//     }
//     return i;
// }

// static int _write(struct _reent *r, void *p, const char *ptr, int len)
// {
//     int i = 0;
//     while (i++ < len) {
//         if (HAL_UART_Transmit(UARTHANDLE, (uint8_t *)ptr, 1, SHELLUART_TIMEOUT_MS) != HAL_OK)
//             break;
//     }

//     return i;
// }

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART1 and Loop until the end of transmission */
    HAL_UART_Transmit(UARTHANDLE, (uint8_t *)&ch, 1, 200);

    return ch;
}

GETCHAR_PROTOTYPE
{
    uint8_t ch;
    HAL_UART_Receive(UARTHANDLE, &ch, 1, HAL_MAX_DELAY);

    /* Echo character back to console */
    // HAL_UART_Transmit(UARTHANDLE, &ch, 1, HAL_MAX_DELAY);

    /* And cope with Windows */
    // if (ch == '\r')
    // {
    //     uint8_t ret = '\n';
    //     HAL_UART_Transmit(UARTHANDLE, &ret, 1, HAL_MAX_DELAY);
    // }

    return ch;
}

#if defined(__IAR_SYSTEMS_ICC__)
/** @brief Sends a character to serial port
 * @param ch Character to send
 * @retval Character sent
 */
int uartSendChar(int ch)
{
    HAL_UART_Transmit(UARTHANDLE, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}
/** @brief Receives a character from serial port
 * @param None
 * @retval Character received
 */
int uartReceiveChar(void)
{
    uint8_t ch;
    HAL_UART_Receive(UARTHANDLE, &ch, 1, HAL_MAX_DELAY);

    /* Echo character back to console */
    // HAL_UART_Transmit(UARTHANDLE, &ch, 1, HAL_MAX_DELAY);

    /* And cope with Windows */
    // if (ch == '\r')
    // {
    //     uint8_t ret = '\n';
    //     HAL_UART_Transmit(UARTHANDLE, &ret, 1, HAL_MAX_DELAY);
    // }

    return ch;
}

/** @brief IAR specific low level standard input
 * @param Handle IAR internal handle
 * @param Buf Buffer where to store characters read from stdin
 * @param Bufsize Number of characters to read
 * @retval Number of characters read
 */
size_t __read(int Handle, unsigned char *Buf, size_t Bufsize)
{
    int i;

    if (Handle != 0)
    {
        return -1;
    }

    for (i = 0; i < Bufsize; i++)
        Buf[i] = uartReceiveChar();

    return Bufsize;
}

/** @brief IAR specific low level standard output
 * @param Handle IAR internal handle
 * @param Buf Buffer containing characters to be written to stdout
 * @param Bufsize Number of characters to write
 * @retval Number of characters read
 */
size_t __write(int Handle, const unsigned char *Buf, size_t Bufsize)
{
    int i;

    if (Handle != 1 && Handle != 2)
    {
        return -1;
    }

    for (i = 0; i < Bufsize; i++)
        uartSendChar(Buf[i]);

    return Bufsize;
}
#endif
