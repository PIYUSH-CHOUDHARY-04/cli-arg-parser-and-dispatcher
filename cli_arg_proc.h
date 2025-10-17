#ifndef __CLI_ARG_PROC_H__
#define __CLI_ARG_PROC_H__

/**
 * @file cli_arg_proc.h
 * @brief Declares prototypes for command line argument handling, let's have a brief look at how the CLI args 
 * 	  are passed to the program :
 * 	  Any option passed must be of form '-arg' and for input with argument, use the form '-arg=X', X can be anything like float or string or int etc
 * 	  All CLI args in details are mentioned below :
 * 	  [#] -addaccount : Adds more google drive accounts by asking for new json file path.
 * 	  [#]	// more to be added....
 */

/**
 * @brief Header file inclusions
 */
#include<stdio.h>
#include<string.h>

/**
 * @brief General and function specific macros
 */
// General macros
#define MAX_CLI_ARGS	32
#define CLI_ARG_COUNT	3

// Arg macros
#define ARG1    "-arg1"
#define ARG2    "-arg2"
#define ARG3    "-arg3"

#define SIZEOF_CSTR(x) (sizeof(x)-(1))  // excluding the null terminator from string length to get char only size 

// cli_arg_dispatcher :
#define EXTRAARG	-1
#define EINVALARG	-2
#define EDUPLARG	-3
#define EHANDLER	-4

/**
 * @brief Variable declarations
 */
extern const char* cli_arg_table[CLI_ARG_COUNT];
extern const int (*fptr_arr[CLI_ARG_COUNT])(void*); // array of function pointers

/**
 * @brief Adds more google drive account for uploading to different drives
 * @return returns 0 on success
 */
int func_arg1(void*);
int func_arg2(void*);
int func_arg3(void*);

/**
 *
 * more functions here....
 *
 */

/**
 * @brief Dispatches command line argument handlers depending on the arguments passed 
 * @param param1 passes the number of argument in the argv vector
 * @param param2 passes the pointer to the argument string array
 * @return returns 0 on success else failed
 */
int cli_arg_dispatcher_lib(int argc, char** argv);

#endif /**< __CLI_ARG_PROC_H__ */

