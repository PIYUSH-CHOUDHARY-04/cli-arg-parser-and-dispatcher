#include<stdio.h>
#include "cli_arg_proc.h"

#define SIZE sizeof("PIYUSH_CHOUDHARY")

int main(int argc, char** argv){
	printf("size : %ld\n", SIZE);
	if(cli_arg_dispatcher(argc, argv)!=0){
		printf("cli_arg_dispatcher() failed.\n");
	}
}
