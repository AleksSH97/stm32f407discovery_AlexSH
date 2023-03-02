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


void console_init(void);
void console_start(void);
char console_get_char(void);
void console_clear_screen_setup(void);

int console_execute(microrl_t *microrl_ptr, int argc, const char * const *argv);
int console_buff(microrl_t *microrl_ptr, int argc, const char * const *argv);
int console_visualizer(microrl_t *microrl_ptr, int argc, const char * const *argv);
char **console_complete(microrl_t *microrl_ptr, int argc, const char * const *argv);

void console_get_version(char* ver_str);
void console_insert_char(char ch);

void console_print_help(microrl_t *microrl_ptr);
void console_print_buff(microrl_t *microrl_ptr);
void console_print_welcome(microrl_t *microrl_ptr);
void console_print_visualizer(microrl_t *microrl_ptr);
void console_sigint(microrl_t *microrl_ptr);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CONSOLE_H_ */
