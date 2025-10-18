#include "cli_arg_proc.h"

/**
 * @brief Variable initialization
 */
const char* cli_arg_table[CLI_ARG_COUNT]={ARG1, ARG2, ARG3};
const int cli_args_size[CLI_ARG_COUNT]={SIZEOF_CSTR(ARG1), SIZEOF_CSTR(ARG2), SIZEOF_CSTR(ARG3)};
const int (*fptr_arr[CLI_ARG_COUNT])(void*)={func_arg1, func_arg2, func_arg3};

/**
 * @brief Adds more google drive account for uploading to different drives
 * @return returns 0 on success
 */
int func_arg1(void*){ printf("Hello from func_arg1\n"); return 0; }
int func_arg2(void*){ printf("Hello from func_arg2\n"); return 0; }
int func_arg3(void*){ printf("Hello from func_arg3\n"); return 0; }

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
int cli_arg_dispatcher_lib(int argc, char** argv){
	// need to read from the second argument since first one always the name of the program itself.
	if(argc<=1){
		return 0;
	}
	
#if COMPILE_FLAG0 == LOGGING
	printf("relation array being created...\n");
#endif
	
	char relation[CLI_ARG_COUNT];
        memset(relation, 0x00, CLI_ARG_COUNT);	
	// this array's elements in sequence represents the index in cli_arg_table and the values stored in the array represents the index in argv



	// let's first validate all the CLI args to avoid any inconsistent program state if any one of the cli arg is wrong in multi arg passing, while validating, we'll store the index of the matched string so that later we can just lookup and dispatch without re-parsing the entire CLI args with cli_arg_table

	char valid_arg_tracer=0;
#if COMPILE_FLAG0 == LOGGING
	char unknown_arg_tracer=0;
#endif
	char dupl_arg_found_flag=0;
	for(int i=0;i<argc-1;i++){
#if COMPILE_FLAG0 == LOGGING
		printf("parsing argv[%d] : %s ...\n",i+1,argv[i+1]);
#endif
		valid_arg_tracer=0;
		for(int j=0;j<CLI_ARG_COUNT;j++){
#if COMPILE_FLAG0 == LOGGING
			printf("comparing argv[%d] %s with cli_arg_table[%d] %s...\n", i+1, argv[i+1], j, cli_arg_table[j]);
#endif
			if(strncmp(argv[i+1], cli_arg_table[j], cli_args_size[j])==0){
				// CLI arg string matched, i_th arg from the list of cli provided arg list is correct.
				valid_arg_tracer=1;
				// let's check if corresponding bit is already set, if yes this means CLI arg duplication else not.
				if((relation[j])==0){
					// no duplication
					relation[j]=i+1;
                   			 // since i_th arg matched, i_th bit is set in index_in_cli_arg_table
                     
				}else{
					// arg duplication, setting flag;
					dupl_arg_found_flag++;
					// setting index_in_cli_arg_table_dupl
					if(relation[j]>0){ relation[j]=(-1)*relation[j]; }
				}	
				
				break;
			}
		}
#if COMPILE_FLAG0 == LOGGING
		if(valid_arg_tracer!=1){	
		// entire array searched, but CLI arg doesnt matched, thus invalid CLI arg
			printf("Unknown argument %d : %s\n", i+1, argv[i+1]);
			unknown_arg_tracer++;
		}
#endif
	}
#if COMPILE_FLAG0 == LOGGING
	if(unknown_arg_tracer!=0){
		printf("Total unknown arguments : %d\n",unknown_arg_tracer);
		printf("Accepted CLI arguments are listed below :\n[\n");
		for(int k=0;k<CLI_ARG_COUNT;k++){
			printf("[*] %s\n",cli_arg_table[k]);
		}
		printf("]\n");
	}
#endif
	if(dupl_arg_found_flag!=0){

#if COMPILE_FLAG0 == LOGGING
		printf("Duplicated argument found : \n");
		for(int l=0;l<CLI_ARG_COUNT;l++){
			if((relation[l]<0)){
				printf("%s\n",cli_arg_table[l]);
			}
		}
#endif
		return EDUPLARG;
	}

#if COMPILE_FLAG0 == LOGGING
	printf("dispatching...\n");
#endif	

#if COMPILE_FLAG0 == LOGGING
	for(int p=0;p<CLI_ARG_COUNT;p++){
		printf("relation[%d] : %d\n", p, relation[p]);
	}
#endif	
	// all CLI args parsed, corresponding indices are stored in index_in_cli_arg_table
	for(int m=0;m<argc-1;m++){
		if(relation[m]>0){

#if COMPILE_FLAG0 == LOGGING
			printf("current iter index in argv : %d\n", m);
			// corresponding index in argv found, executing the handler.
			// now checking whether the provided arg is switch or a variable type i.e. -switch or -var=X
#endif
			if(strlen(argv[relation[m]])==cli_args_size[m]){
				// its a switch, executing handler as is
#if COMPILE_FLAG0 == LOGGING
				printf("dispatching handler for switch %s...\n", argv[relation[m]]);
#endif
				if((fptr_arr[m])(NULL)!=0){
#if COMPILE_FLAG0 == LOGGING
					printf("Handler ar index %d and address %p failed.\n", m, fptr_arr[m]);
#endif
					return EHANDLER;
				}
				continue;
			}else{
			
				// its not a switch, value based arg i.e. -var=X
#if COMPILE_FLAG0 == LOGGING
				printf("dispatching handler for var_arg %s...\n", argv[relation[m]]);
#endif
				if(((fptr_arr[m])((void*)(argv[relation[m]]+cli_args_size[m]+1) )!=0)){
					// +1 to make the pointer to point beyond '=' i.e. to next char whatever it is
#if COMPILE_FLAG0 == LOGGING
					printf("Handler at index %d and address %p failed.\n", m, fptr_arr[m]);
#endif
					return E_HANDLER;
				}

			}
		}
	}
	return 0;
}

