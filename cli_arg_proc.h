#ifndef __CLI_ARG_PROC_H__
#define __CLI_ARG_PROC_H__

#define COMPILE_FLAG0	NOLOGGING	// LOGGING (allows execution of printf() statements for debugging) or NOLOGGING (removes the printf() and relevant debugging ceode from being compiled)

/**
 * @file cli_arg_proc.h
 * @brief Declares prototypes for command line argument handling of type switches (-switch) and key-value pairs (-key=value) for you library
 * 	  let's have a brief look at how the CLI args are passed to the program :
 * 	  Any option passed must be of form '-arg' and for input with argument, use the form '-arg=X', X can be anything like float or string or int etc
 *
 * 	  User can add custom CLI arguments and handlers for them as follow:
 *
 * 	  [#] In cli_arg_proc.h file, increase the CLI_ARG_COUNT macro value by the units you of CLI args to be added
 * 	  [#] Add ARGn macro in cli_arg_proc.h file as the CLI arg string (for both switches and key=value pairs, just write the arg string as "-lib-argX", arg type differentiating logic is taken care of)
 * 	  [#] In cli_arg_proc.c (i.e. in this file), add ARGn macro in the cli_arg_table array
 * 	  [#] In cli_arg_proc.c (i.e. in this file), add SIZEOF_CSTR(ARGn) in cli_arg_size array
 * 	  [#] In cli_arg.proc.h file, add function prototype with syntax int func_argn(void*), function name can be though of choice but for easy tracking, its good to be indexed
 * 	  [#] In cli_arg_proc.c (i.e. in this file), add definition of the func_argn handler declared in cli_arg_proc.h
 * 	  [#] In cli_arg_proc.c (i.e. in this file), add func_argn function pointer in fptr_arr array
 *
 *
 * 	  Note that the library specific CLI ARGs must be as "-lib-ARGn" i.e. "-lib-" must be used at the beginning of the ARGn for clear distinction from typical user program specific CLI ARGs, 
 * 	  hence, avoid using the ARGS starting like "-lib-" for you user programs for both safety and understanding point of view
 *
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
#define EDUPLARG	-1
#define EHANDLER	-2

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

