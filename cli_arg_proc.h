#ifndef __CLI_ARG_PROC_H__
#define __CLI_ARG_PROC_H__

#define COMPILE_FLAG0	NO_LOGGING	// NO_LOGGING (for disabling printf() statements to use in program) or LOGGING (for keeping printf() statements for debugging)

/**
 * @file cli_arg_proc.h
 * @brief Declares prototypes for command line argument handling of type switches (-switch) and key-value pairs (-key=value), 
 * 	  let's have a brief look at how the CLI args are passed to the program :
 * 	  Any option passed must be of form '-arg' and for input with argument, use the form '-arg=X', X can be anything like float or string or int etc
 * 	  
 * 	  User can add custom CLI arguments and handlers for them as follow:
 *
 * 	  [#] In cli_arg_proc.h file, increase the CLI_ARG_COUNT macro value by the units you of CLI args to be added
 * 	  [#] Add ARGn macro in cli_arg_proc.h file as the CLI arg string (for both switches and key=value pairs, just write the arg string as "-argX", arg type differentiating logic is taken care of)
 * 	  [#] In cli_arg_proc.c (i.e. in this file), add ARGn macro in the cli_arg_table array
 * 	  [#] In cli_arg_proc.c (i.e. in this file), add SIZEOF_CSTR(ARGn) in cli_arg_size array
 * 	  [#] In cli_arg.proc.h file, add function prototype with syntax int func_argn(void*), function name can be though of choice but for easy tracking, its good to be indexed
 * 	  [#] In cli_arg_proc.c (i.e. in this file), add definition of the func_argn handler declared in cli_arg_proc.h
 * 	  [#] In cli_arg_proc.c (i.e. in this file), add func_argn function pointer in fptr_arr array
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
#define CLI_ARG_COUNT	3

// Arg macros
#define ARG1    "-arg1"
#define ARG2    "-arg2"
#define ARG3    "-arg3"

#define SIZEOF_CSTR(x) (sizeof(x)-(1))  // excluding the null terminator from string length to get char only size 

// cli_arg_dispatcher :
#define EINVALARG	-1
#define EDUPLARG	-2
#define EHANDLER	-3

/**
 * @brief Variable declarations
 */
extern const char* cli_arg_table[CLI_ARG_COUNT];    // array to hold ARGn strings
extern const int (*fptr_arr[CLI_ARG_COUNT])(void*); // array of function pointers

/**
 * @brief Adds more handler prototypes
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

