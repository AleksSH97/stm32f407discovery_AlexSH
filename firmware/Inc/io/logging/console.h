/*
 * console.h
 *
 *  Created on: 5 янв. 2023 г.
 *      Author: АлексанDOOR
 */

#ifndef CONSOLE_H_
#define CONSOLE_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>

#include "uart.h"
#include "log.h"
#include "microrl.h"
#include "microrl_config.h"


void ConsoleInit(void);
void ConsoleStart(void);
char ConsoleGetChar(void);
void ConsoleClearScreenSetup(void);

int ConsoleExecute(microrl_t *microrl_ptr, int argc, const char * const *argv);
int ConsoleBuff(microrl_t *microrl_ptr, int argc, const char * const *argv);
int ConsoleVisualizer(microrl_t *microrl_ptr, int argc, const char * const *argv);
char **ConsoleComplete(microrl_t *microrl_ptr, int argc, const char * const *argv);

void ConsoleGetVersion(char* ver_str);
void ConsoleInsertChar(char ch);

void ConsolePrintHelp(microrl_t *microrl_ptr);
void ConsolePrintBuff(microrl_t *microrl_ptr);
void ConsolePrintWelcome(microrl_t *microrl_ptr);
void ConsolePrintVisualizer(microrl_t *microrl_ptr);
void ConsoleSigint(microrl_t *microrl_ptr);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CONSOLE_H_ */
