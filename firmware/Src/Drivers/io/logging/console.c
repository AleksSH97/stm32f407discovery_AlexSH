/*
 * console.c
 *
 *  Created on: 05 jan. 2023 г.
 *      Author: Aleksandr Shabalin
 */

#include "console.h"

#define _STM32_DEMO_VER             "1.0"

#define _ENDLINE_SEQ                "\r\n"

/* Definition commands word */
#define _CMD_HELP                   "help"
#define _CMD_CLEAR                  "clear"
#define _CMD_LOGIN                  "login"
#define _CMD_LOGOUT                 "logout"

#define _CMD_BUFF                   "buff"
#define _CMD_RESET                  "reset"
#define _CMD_FREE                   "free"
#define _CMD_CHECK                  "check"
#define _CMD_EXIT                   "exit"
/* Arguments for set/clear */
#define _SCMD_RD                    "?"
#define _SCMD_SAVE                  "save"

#define _NUM_OF_CMD                 8
#define _NUM_OF_SETCLEAR_SCMD       2

#if MICRORL_CFG_USE_ECHO_OFF
#define SESSION_ADMIN_LOGIN        "admin"
#define SESSION_ADMIN_PASSW        "17391739"
#endif /* MICRORL_CFG_USE_ECHO_OFF */

#if MICRORL_CFG_USE_ECHO_OFF
/* Session status flags */
uint8_t  logged_in = 0;
uint8_t  passw_in = 0;
#endif /* MICRORL_CFG_USE_ECHO_OFF */

static void console_print(microrl_t *microrl_ptr, const char *str);

microrl_t microrl;
microrl_t *microrl_ptr = &microrl;

char *keyword[] = {_CMD_HELP, _CMD_CLEAR, _CMD_LOGIN, _CMD_LOGOUT, _CMD_BUFF, _CMD_RESET, _CMD_FREE, _CMD_EXIT};    //available  commands
char *read_save_key[] = {_SCMD_RD, _SCMD_SAVE};            // 'read/save' command arguments
char *compl_word [_NUM_OF_CMD + 1];                        // array for completion

void console_init(void)
{
    microrl_init(microrl_ptr, console_print, console_execute);

#if MICRORL_CFG_USE_COMPLETE
    /* Set callback for auto-completion */
    microrl_set_complete_callback(microrl_ptr, console_complete);
#endif /* MICRORL_CFG_USE_COMPLETE */

#if MICRORL_CFG_USE_CTRL_C
    /* Set callback for Ctrl+C handling */
    microrl_set_sigint_callback(microrl_ptr, console_sigint);
#endif /* MICRORL_CFG_USE_CTRL_C */
}

void console_insert_char(char ch)
{
    microrl_processing_input(microrl_ptr, &ch, sizeof(ch));
}

void console_print(microrl_t *microrl_ptr, const char *str)
{
    UNUSED(microrl_ptr);
    log_printf_cont("%s", str);
}

char console_get_char(void)
{
   return (char)data_uart.console_input;
}

#if MICRORL_CFG_USE_ECHO_OFF
int console_execute_main(microrl_t* microrl_ptr, int argc, const char* const *argv) {
#else
int console_execute(microrl_t *microrl_ptr, int argc, const char * const *argv) {
#endif /* MICRORL_CFG_USE_ECHO_OFF || __DOXYGEN__ */
    int i = 0;

    while (i < argc) {
        if (strcmp(argv[i], _CMD_HELP) == 0) {
            console_print_help(microrl_ptr);        // print help
        }
        else if (strcmp(argv[i], _CMD_CLEAR) == 0) {
            console_print(microrl_ptr, "\033[2J" _ENDLINE_SEQ);    // ESC seq for clear entire screen
            console_print(microrl_ptr, "\033[H" _ENDLINE_SEQ);     // ESC seq for move cursor at left-top corner
        }
        else if (strcmp(argv[i], _CMD_LOGOUT) == 0) {
            console_print(microrl_ptr, "\tBye!" _ENDLINE_SEQ);
            microrl_set_execute_callback(microrl_ptr, console_execute);
        }
        else if (strcmp(argv[i], _CMD_BUFF) == 0) {
            console_print(microrl_ptr, "\tChoose your action with buffer: " _ENDLINE_SEQ);
            console_print_buff(microrl_ptr);
            microrl_set_execute_callback(microrl_ptr, console_buff);
        }
        else {
            console_print(microrl_ptr, "command: '");
            console_print(microrl_ptr, (char*)argv[i]);
            console_print(microrl_ptr, "' Not found." _ENDLINE_SEQ);
        }
        i++;
    }

    return 0;
}

int console_execute(microrl_t *microrl_ptr, int argc, const char * const *argv)
{
    int i = 0;

    while (i < argc) {
        if (strcmp(argv[i], _CMD_LOGIN) == 0) {
            if (++i < argc) {
                if (strcmp(argv[i], SESSION_ADMIN_LOGIN) == 0) {
                    console_print(microrl_ptr, "\tEnter password:" _ENDLINE_SEQ);
                    microrl_set_echo(microrl_ptr, MICRORL_ECHO_ONCE);
                    passw_in = 1;
                    return 0;
                }
                else {
                    console_print(microrl_ptr, "\tLogin name doesn't registered. Try again." _ENDLINE_SEQ);
                    indication_led_error();
                    return 1;
                }
            }
            else {
                console_print(microrl_ptr, "\tEnter your login after 'login' command." _ENDLINE_SEQ);
                return 0;
            }
        }
        else if (passw_in == 1) {
            if (strcmp(argv[i], SESSION_ADMIN_PASSW) == 0) {
                console_print(microrl_ptr, "\tLogged in successfully." _ENDLINE_SEQ);
                passw_in = 0;
                logged_in = 1;
                microrl_set_execute_callback(microrl_ptr, console_execute_main);
                return 0;
            }
            else {
                console_print(microrl_ptr, "\tWrong password. Try again." _ENDLINE_SEQ);
                indication_led_error();
                passw_in = 0;
                return 1;
            }
        }
        else {
            console_print(microrl_ptr, "\tYou need to login first!" _ENDLINE_SEQ);
            console_print(microrl_ptr, "\tlogin YOUR_LOGIN"_ENDLINE_SEQ);
            indication_led_error();
            return 1;
        }
        i++;
    }

    return 0;
}

int console_buff(microrl_t *microrl_ptr, int argc, const char * const *argv)
{
    int i = 0;

    while (i < argc) {
        if (strcmp(argv[i], _CMD_RESET) == 0) {
            lwrb_reset(&debug_uart.lwrb);
            console_print(microrl_ptr, "\tBuffer successfully reseted" _ENDLINE_SEQ);
        }
        else if (strcmp(argv[i], _CMD_FREE) == 0) {
            lwrb_free(&debug_uart.lwrb);
            console_print(microrl_ptr, "\tBuffer successfully free" _ENDLINE_SEQ);
        }
        else if (strcmp(argv[i], _CMD_CHECK) == 0) {
            console_print(microrl_ptr, "\tHere's your buffer:  ");
            for (int i = 0; i < UART_BUFF_SIZE; i++) {
                log_printf("%d ", data_uart.lwrb.buff[i]);
            }
            console_print(microrl_ptr, "\t " _ENDLINE_SEQ);
        }
        else if (strcmp(argv[i], _CMD_EXIT) == 0) {
            console_print(microrl_ptr, "\tBack to main menu" _ENDLINE_SEQ _ENDLINE_SEQ _ENDLINE_SEQ);
            console_print_help(microrl_ptr);
            microrl_set_execute_callback(microrl_ptr, console_execute_main);
        }
        else {
            console_print(microrl_ptr, "\tUndefined command" _ENDLINE_SEQ);
            indication_led_error();
            console_print_buff(microrl_ptr);
        }
        i++;
    }

    return 0;
}

#if MICRORL_CFG_USE_CTRL_C || __DOXYGEN__
/**
 * \brief           Ctrl+C terminal signal function
 * \param[in]       mrl: \ref microrl_t working instance
 */
void console_sigint(microrl_t *microrl_ptr)
{
    console_print(microrl_ptr, "^C is caught!"_ENDLINE_SEQ);
}

//*****************************************************************************
// completion callback for microrl library
char **console_complete(microrl_t *microrl_ptr, int argc, const char * const *argv)
{
    UNUSED(microrl_ptr);
    int j = 0;

    compl_word[0] = NULL;

    if (argc == 1) {
        char *bit = (char *)argv[argc - 1];

        for (int i = 0; i < _NUM_OF_CMD; i++) {
            if (strstr(keyword[i], bit) == keyword[i]) {
                compl_word[j++] = keyword[i];
            }
        }
    }
    else if ((argc > 1) && ((strcmp (argv[0], _CMD_LOGIN) == 0))) {  /*If command needs subcommands */
        /* Iterate through subcommand */
        for (int i = 0; i <  _NUM_OF_SETCLEAR_SCMD; i++) {
            if (strstr (read_save_key[i], argv[argc-1]) == read_save_key [i]) {
                compl_word[j++] = read_save_key[i];
            }
        }
    }
    else { /* If there is no token in cmdline, just print all available token */
        for (; j < _NUM_OF_CMD; j++) {
            compl_word[j] = keyword[j];
        }
    }
    compl_word[j] = NULL; /* Last ptr in array always must be NULL */

    return compl_word;
}

#endif /* MICRORL_CFG_USE_COMPLETE || __DOXYGEN__ */

void console_print_help(microrl_t *microrl_ptr)
{
    char ver_str[6] = {0};
    console_get_version(ver_str);

    console_print(microrl_ptr, "MicroRL v");
    console_print(microrl_ptr, ver_str);
    console_print(microrl_ptr, " library DEMO v");
    console_print(microrl_ptr, _STM32_DEMO_VER);
    console_print(microrl_ptr, _ENDLINE_SEQ);

#if MICRORL_CFG_USE_ECHO_OFF
    if (!logged_in) {
        console_print(microrl_ptr, "\tlogin YOUR_LOGIN      - 'admin' in this example"_ENDLINE_SEQ);
    }
#endif /* MICRORL_CFG_USE_ECHO_OFF */

    console_print(microrl_ptr, "List of commands:"_ENDLINE_SEQ);
    console_print(microrl_ptr, "\tclear               - clear screen"_ENDLINE_SEQ);
    console_print(microrl_ptr, "\tlogout              - end session"_ENDLINE_SEQ);
    console_print(microrl_ptr, "\tbuff                - buff configuring menu"_ENDLINE_SEQ);

#if MICRORL_CFG_USE_COMPLETE
    console_print(microrl_ptr, "Use TAB key for completion"_ENDLINE_SEQ _ENDLINE_SEQ);
#endif /* MICRORL_CFG_USE_COMPLETE */
}

void console_print_buff(microrl_t *microrl_ptr)
{
    console_print(microrl_ptr, "List of buff commands:"_ENDLINE_SEQ);
    console_print(microrl_ptr, "\treset               - reset buffer"_ENDLINE_SEQ);
    console_print(microrl_ptr, "\tfree                - free buff memory"_ENDLINE_SEQ);
    console_print(microrl_ptr, "\tcheck               - check your buff"_ENDLINE_SEQ);
    console_print(microrl_ptr, "\texit                - back to main menu"_ENDLINE_SEQ _ENDLINE_SEQ);
}

void console_get_version(char* ver_str)
{
    uint32_t ver = microrl_get_version();

    ver_str[0] = (char)((ver >> 16) & 0x000000FF) + '0';
    ver_str[1] = '.';
    ver_str[2] = (char)((ver >> 8) & 0x000000FF) + '0';
    ver_str[3] = '.';
    ver_str[4] = (char)(ver & 0x000000FF) + '0';
}

