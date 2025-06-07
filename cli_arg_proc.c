#include "cli_arg_proc.h"

/**
 * @brief Variable initialization
 */
const char* cli_arg_table[CLI_ARG_COUNT]={ADDACCOUNT};
const int (*fptr_arr[CLI_ARG_COUNT])(void)={add_account};

/**
 * @brief Adds more google drive account for uploading to different drives
 * @return returns 0 on success
 */
int add_account(void){
	return 0;

}

/**
 * more functions here....
 *
 */




/**
 * @brief Dispatches command line argument handlers depending on the arguments passed 
 * @param param1 passes the number of argument in the argv vector
 * @param param2 passes the pointer to the argument string array
 * @return returns 0 on success else failed
 */
int cli_arg_dispatcher(int argc, char** argv){
	// need to read from the second argument since first one always the name of the program itself.
	if(argc<=1){
		return 0;
	}
	if((argc-1)>CLI_ARG_COUNT){
		printf("Maximum argument supported : %d, Given : %d\n", CLI_ARG_COUNT, argc-1);
		return EXTRAARG;
	}
	
	// let's first validate all the CLI args to avoid any inconsistent program state if any one of the cli arg is wrong in multi arg passing, while validating, we'll store the index of the matched string so that later we can just lookup and dispatch without re-parsing the entire CLI args with cli_arg_table
	unsigned int cli_arg_index=0x00000000;
	unsigned int cli_arg_index_dupl=0x00000000;
	char flag0=0;
	char flag1=0;
	char duplic_flag=0;
	for(int i=1;i<argc;i++){
		flag0=0;
		for(int j=0;j<CLI_ARG_COUNT;j++){
			if(strcmp(argv[i], cli_arg_table[j])==0){
				// CLI arg string matched, i_th arg correct.
				flag0=1;
				// let's check if corresponding bit is already set, if yes this means CLI arg duplication else not.
				if(((cli_arg_index)&(1<<j))==0){
					// no duplication
					cli_arg_index|=(1<<j);
				}else{
					// arg duplication, setting flag;
					duplic_flag++;
					// setting cli_arg_index_dupl
					cli_arg_index_dupl|=(1<<j);
				}	
				
				break;
			}
		}
		if(flag0!=1){	
		// entire array searched, but CLI arg doesnt matched, thus invalid CLI arg
			printf("Invalid argument %d : %s\n",i,argv[i]);
			flag1++;
		}
	}
	if(flag1!=0){
		printf("Total invalid arguments : %d\n",flag1);
		printf("Accepted CLI arguments are listed below :\n[\n");
		for(int k=0;k<CLI_ARG_COUNT;k++){
			printf("[*] %s\n",cli_arg_table[k]);
		}
		printf("]\n");
		return EINVALARG;
	}
	if(duplic_flag!=0){
		printf("Duplicated argument found : \n");
		for(int l=0;l<CLI_ARG_COUNT;l++){
			if(((cli_arg_index_dupl)&(1<<l))!=0){
				printf("%s\n",cli_arg_table[l]);
			}
		}
		return EDUPLARG;
	}

	// all CLI args parsed, corresponding indices are stored in cli_arg_index
	for(int m=0;m<CLI_ARG_COUNT;m++){
		if(((cli_arg_index)&(1<<m))!=0){
			// corresponding bit found, executing the handler.
			if((fptr_arr[m])()!=0){
				printf("Handler at index %d and address %p failed.\n",m,fptr_arr[m]);
				return EHANDLER;
			}
		}
	}

	return 0;
}
