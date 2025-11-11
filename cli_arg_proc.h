#ifndef __CLI_ARG_PROC_H__
#define __CLI_ARG_PROC_H__

/**
 * @file cli_arg_proc.h
 * @brief Declares prototype for command line argument handling of type switches (-switch) and key-value pairs (-key=value) for CLI software development
 *
 * 	  Any option passed must be of form '-arg' at minimum whether its switch type or value type,
 *	  For switch type, '-arg' is enough; for value type, '-arg=' is minimum, pointer to char after '=' is passed to handler functions for self value retrieval
 *
 * 	  User can add custom CLI arguments and handlers for them as follow:
 *
 * 	  [#] Define your function in handlers.c, if needed, put there the private macros, structs, vars etc, 
 * 	      and try to follow the name convention as func_argX otherwise put the same in arg_fptr field of argbox element of ARGS array
 * 	  [#] Put handler prototypes in handlers.h
 * 	  [#] In cli_arg_proc.c file, set CLI_ARG_COUNT as per needs, marked as [---MODIFIABLE---]
 * 	  [#] In cli_arg_proc.c file, modify MAX_IDX_COUNT as per needs, marked as [---MODIFIABLE---]
 * 	  [#] In cli_arg_proc.c file, change the arg macros as per needs, marked as [---MODIFIABLE---] 
 * 	  [#] Add more entires in ARGS[] array as per needs, marked as [---MODIFIABLE---]
 *	  [#] Keep the arg_use_check and arg_misuse fields always 0, marked as [---SET TO 0 ALWAYS---]
 *	  [#] Arguments reusability can be set by setting a count in arg_reuse_count while initializing the ARGS
 *	  [#] Further, logging for debug purpose can be enabled or disabled by setting LOG_ENABLE to TRUE or FALSE in handlers.h, marked as [---MODIFIABLE---]
 *
 */

/**
 *@brief headers
 */
#include <stdio.h>
#include <string.h>



/**
 * @brief Dispatches command line argument handlers depending on the arguments passed 
 * @param param1 passes the number of argument in the argv vector
 * @param param2 passes the pointer to the argument string array
 * @return returns 0 on success else failed
 */
int cli_arg_dispatcher(int argc, char** argv);

#endif /**< __CLI_ARG_PROC_H__ */

