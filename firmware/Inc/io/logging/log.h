/*
 * log.h
 *
 *  Created on: 20 дек. 2022 г.
 *      Author: АлексанDOOR
 */

#ifndef LOG_H_
#define LOG_H_

#include <stdarg.h>
#include <stdio.h>

#include "lwprintf/lwprintf.h"
#include "microrl.h"
#include "console.h"
#include "io_system.h"

#include "FreeRTOS.h"
#include "cmsis_os2.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************************************************************/
/* Public defines ----------------------------------------------------------- */
/******************************************************************************/
/* List of supported ESC symbol colors */
#define CLR_RD         "\033[31m"               /* Red color      */
#define CLR_GR         "\033[32m"               /* Green color    */
#define CLR_YL         "\033[33m"               /* Yellow color   */
#define CLR_BL         "\033[34m"               /* Blue color     */
#define CLR_PR         "\033[35m"               /* Purple color   */
#define CLR_MG         "\033[36m"               /* Magenta color  */
#define CLR_WT         "\033[37m"               /* White color    */
#define CLR_DEF        "\033[0m"                /* Default color  */

#define CLR_CLR        "\033[2J\033[H\033[0m"   /* Clear terminal */




/******************************************************************************/
/* Public variables --------------------------------------------------------- */
/******************************************************************************/
extern osMessageQueueId_t consoleQueueHandle;
extern osMessageQueueId_t logsQueueHandle;


/******************************************************************************/
/* Public functions --------------------------------------------------------- */
/******************************************************************************/
#define    PrintfLogsCRLF(fmt, ...)               do { PrintfLogs((fmt), ## __VA_ARGS__); PrintfLogs("\r\n"); } while (0)

#define    PrintfLogsCont(fmt, ...)               PrintfLogs((fmt), ## __VA_ARGS__)

#define    PrintfConsoleCRLF(fmt, ...)           do { PrintfConsole((fmt), ## __VA_ARGS__); PrintfConsole("\r\n"); } while (0)

#define    PrintfConsoleCont(fmt, ...)           PrintfConsole((fmt), ## __VA_ARGS__)

void LogsTaskStart(void *argument);
void LogClearQueues(void);
void LogInit(void);
void LogPrintWelcomeMsg(void);
int PrintfLogs(const char *fmt, ...);
int PrintfConsole(const char *fmt, ...);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LOG_H_ */
