#include "cli_arg_proc.h"

int main(int argc, char** argv){
	if(cli_arg_dispatcher(argc, argv)!=0){
		printf("cli_arg_dispatcher() failed.\n");
	}
}
