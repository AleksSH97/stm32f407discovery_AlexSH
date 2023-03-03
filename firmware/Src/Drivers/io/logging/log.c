/*
 * log.c
 *
 *  Created on: 20 дек. 2022 г.
 *      Author: АлексанDOOR
 */

/******************************************************************************/
/* Includes ----------------------------------------------------------------- */
/******************************************************************************/
#include "log.h"
#include "uart.h"

#include "lwprintf/lwprintf.h"


#define LOGS_QUEUE_SIZE            (512U)
#define CONSOLE_QUEUE_SIZE         (512U)
/******************************************************************************/




static int prvLwprintfLogsOut(int ch, lwprintf_t* p);
static int prvLwprintfConsoleOut(int ch, lwprintf_t* p);
/******************************************************************************/




osMessageQueueId_t consoleQueueHandle;
osMessageQueueId_t logsQueueHandle;
/******************************************************************************/





const osMessageQueueAttr_t consoleQueueAttributes = {
        .name = "consoleQueue",
};

const osMessageQueueAttr_t logsQueueAttributes = {
        .name = "logsQueue",
};
/******************************************************************************/





static lwprintf_t console;
static lwprintf_t logs;
/******************************************************************************/




void LogInit(void)
{
    consoleQueueHandle = osMessageQueueNew(512, sizeof(uint8_t), &consoleQueueAttributes);
    logsQueueHandle = osMessageQueueNew(512, sizeof(uint8_t), &logsQueueAttributes);

    lwprintf_init_ex(&console, prvLwprintfConsoleOut);
    lwprintf_init_ex(&logs, prvLwprintfLogsOut);
}
/******************************************************************************/




void LogClearQueues(void)
{
    osMessageQueueReset(logsQueueHandle);
    osMessageQueueReset(consoleQueueHandle);
}
/******************************************************************************/




int PrintfLogs(const char *fmt, ...)
{
    va_list args;
    int len;

    va_start(args, fmt);
    len = lwprintf_vprintf_ex(&logs, fmt, args);
    va_end(args);

    return (len);
}
/******************************************************************************/




int PrintfConsole(const char *fmt, ...)
{
    va_list args;
    int len;

    va_start(args, fmt);
    len = lwprintf_vprintf_ex(&console, fmt, args);
    va_end(args);

    return (len);
}
/******************************************************************************/



static int prvLwprintfLogsOut(int ch, lwprintf_t* p)
{
    uint8_t c = (uint8_t)ch;

    if (c == '\0') {
        return ch;           //to prevent printing '0' in the end of any (char*)
    }

    osMessageQueuePut(logsQueueHandle, &ch, 0, 200);

    return (ch);
}
/******************************************************************************/





static int prvLwprintfConsoleOut(int ch, lwprintf_t* p)
{
    uint8_t c = (uint8_t)ch;

    if (c == '\0') {
        return ch;           //to prevent printing '0' in the end of any (char*)
    }

    osMessageQueuePut(consoleQueueHandle, &ch, 0, 200);

    return (ch);
}
/******************************************************************************/




void LogPrintWelcomeMsg(void)
{
    PrintfLogsCRLF("");
    PrintfLogsCRLF("");
    PrintfLogsCRLF("\tWelcome to STM32F407 Discovery firmware with FreeRTOS");
    PrintfLogsCRLF("");
    PrintfLogsCRLF("");
}
/******************************************************************************/

