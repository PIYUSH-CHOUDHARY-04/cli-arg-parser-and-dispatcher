#ifndef __HANDLERS_H__
#define __HANDLERS_H__

/**
 * @file handlers.h
 * @brief This file contains shared macros, function prototypes and anything else that has to be exposed to other files which
 * 	  is specially utilized by the handler functions
 */

/**
 * @brief Header inclusion
 */
// system inclusion
#include <stdio.h>
#include <string.h>


/**
 * @brief Shared macros
 */
// general macros
#define TRUE            1
#define FALSE           0
#define LOG_ENABLE  TRUE	/* [---MODIFIABLE---] */

// conditional macros
#if LOG_ENABLE == TRUE		
        #define LOG_MSG(fmt, ...)       printf(fmt, ##__VA_ARGS__)
#else   
        #define LOG_MSG(fmt, ...)       void(0)
#endif

// Error code macros
#define EHANDLER 	-64 // generic for now, reserved range [int_min, -64]


/**
 * @brief Function prototypes
 */
int func_arg1(void* ptr1);
int func_arg2(void* ptr2);
int func_arg3(void* ptr3);


#endif 	/* __HANDLERS_H__ */


