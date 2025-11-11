#include "handlers.h"

/**
 * @file handlers.c
 * @brief This file contains handler function specific macros, struct definitions and initializations, static variables, function definitions etc 
 */

/**
 * @brief Private macros
 */



/**
 * @brief Function definitions for handlers
 * @param void pointer to either a non-null char next to '=' if its a value based argument or to a null character if its a switch based argument, 
 * 	  No need to check for NULL pointer inside the function if you are defining it for switch type args otherwise, mandatorily put this check to prevent segfault
 * @return returns 0 on success, negative number on failure
 */
int func_arg1(void* ptr1){
	LOG_MSG("Hello from func1\n");
	return 0;
}
int func_arg2(void* ptr2){
	LOG_MSG("Hello from func2\n");
	printf("arg2 val : %s\n", (char*)ptr2);
	return 0;
}
int func_arg3(void* ptr3){
	LOG_MSG("Hello from func3\n");
	return 0;
}






