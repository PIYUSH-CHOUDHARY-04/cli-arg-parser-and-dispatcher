#include "cli_arg_proc.h"
#include "handlers.h"

/**
 * @brief Private macros
 */
// general macros
#define CLI_ARG_COUNT	3	/* [---MODIFIABLE---] */
#define SIZEOF_CSTR(x)	(sizeof(x)-(1))
#define MAX_IDX_COUNT	7 	/* [---MODIFIABLE---] */  // follows the sequence 7, 15, 23, 31 ... i.e. 8n-1, hence makes the relation struct byte edged i.e. no padding bytes (7->128 max args; 15->2^15 max args; ...)	
#define REUSE_ENABLE	TRUE
#define ARG_USED	1	// represents that arg is used once

// error code macros	(error codes from -1 to -63)
#define EINVALARG	-1
#define EARGMISUSED	-2

// arg macros  [---MODIFIABLE---] 
#define ARG1	"-arg1"
#define ARG2	"-arg2"
#define ARG3 	"-arg3"



/**
 * @brief Structure definitions
 */
struct argbox {
	const char* 		arg_str;
	const short int 	arg_str_size;
	const int		(*arg_fptr)(void*);
	const unsigned int 	arg_reuse	:1;		// REUSE_ENABLE and !REUSE_ENABLE for now
	const unsigned int 	arg_reuse_count :7;		// maximum reuse allowed, 0 not allowed, useful if arg_reuse==REUSE_ENABLE
	int 			arg_use_check	:7;		/* [---SET TO 0 ALWAYS---] */ // will be used by cli_arg_dispatcher() to identify whether its used or not, and how many times
	unsigned int		arg_misuse	:1;		/* [---SET TO 0 ALWAYS---] */ // set only if arg_use_check exceeds arg_reuse_count
};

struct relation {
	unsigned int 		rel_err	:1;		// 0 means no error, 1 refers some kinds error explained by rel_idx field
	unsigned int		rel_idx	:MAX_IDX_COUNT;
};

/**
 * @brief Private variables
 */
static struct argbox ARGS[CLI_ARG_COUNT] = {	/* [---MODIFIABLE---] */
				{ .arg_str=ARG1, .arg_str_size=SIZEOF_CSTR(ARG1), .arg_fptr=func_arg1, .arg_reuse=!REUSE_ENABLE, .arg_reuse_count=0, .arg_use_check=0, .arg_misuse=0 },
				{ .arg_str=ARG2, .arg_str_size=SIZEOF_CSTR(ARG2), .arg_fptr=func_arg2, .arg_reuse= REUSE_ENABLE, .arg_reuse_count=5, .arg_use_check=0, .arg_misuse=0 },
				{ .arg_str=ARG3, .arg_str_size=SIZEOF_CSTR(ARG3), .arg_fptr=func_arg3, .arg_reuse=!REUSE_ENABLE, .arg_reuse_count=0, .arg_use_check=0, .arg_misuse=0 },
				// more can be added as per needs
};





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
	
	LOG_MSG("Creating relation array...\n");
	
	struct relation rel_arr[argc-1];	// one struct relation for each arg except program name itself
        memset(rel_arr, 0x00, (argc-1)*sizeof(struct relation));
	// this array's elements in sequence represents the index of the struct argbox element of the ARGS array thus connecting the arg to its identity



	// let's first validate all the CLI args to avoid any inconsistent program state if any one of the cli arg is wrong in multi arg passing, while validating, we'll store the index of the matched string so that later we can just lookup and dispatch without re-parsing the entire CLI args with cli_arg_table


	char unknown_arg_flag=0;
	char misuse_arg_flag=0;

	for(int i=1; i<argc; i++){	// i refers to ith arg in argv i.e. argv[i], thus droping argv[0] as its the program name itself
		LOG_MSG("parsing argv[%d] : %s ...\n",i,argv[i]);

		int j=0;
		for(j; j<CLI_ARG_COUNT; j++){ // j refers to the jth element of ARGS array with which we are comparing the user passed arg

			LOG_MSG("comparing argv[%d] %s with ARGS[%d] %s...\n", i, argv[i], j, ARGS[j].arg_str);
			if(strncmp(argv[i], ARGS[j].arg_str, ARGS[j].arg_str_size)==0){
				// CLI arg string matched, i_th arg from the list of cli provided arg list is correct.
				
				// let's check if corresponding bit is already set, if yes this means CLI arg duplication else not.
				if(!(ARGS[j].arg_use_check)){
					// arg being found for first time, next time behavior depends on argbox.arg_reuse
                   			// since i_th arg matched jth element, we need to set argbox.arg_use_check 
					ARGS[j].arg_use_check=ARG_USED;
                     
				}else{
					// arg already used, checking whether reuse allowed or not, taking paths accordingly
					// arg duplication
					if(!ARGS[j].arg_reuse || (ARGS[j].arg_reuse && (ARGS[j].arg_use_check)==(ARGS[j].arg_reuse_count))){
						// arg already used maximum times, now setting misuse bit
						ARGS[j].arg_misuse=1;
						misuse_arg_flag++;
					}else
					if(ARGS[j].arg_reuse && ARGS[j].arg_use_check<ARGS[j].arg_reuse_count){
						(ARGS[j].arg_use_check)++;
					}
					// other cases are not possible to happen cuz of code flow design	
				}
				LOG_MSG("Setting index for argv[%d] as %d\n", i, j);
				rel_arr[i-1].rel_idx=j;
				LOG_MSG("index set : %d\n", rel_arr[i].rel_idx);
				break;
			}else{

			// marking the end of the inner for loop, setting j=-1 if no match found
				if(j==CLI_ARG_COUNT){ j=-1; }
			}
		}			
		if(j==-1){	
			// user passed arg not found
			LOG_MSG("argv[%d] didn't matched\n", i);
			rel_arr[i].rel_err=1;
			unknown_arg_flag++;
			LOG_MSG("Current unknown args found : %d\n", unknown_arg_flag);
			//rel_arr[i].rel_idx need not to be altered cuz while dispatch, the error bit will be checked first
		}

	}


	LOG_MSG("rel_arr created, printing it ...\n\n");
#if LOG_ENABLE == TRUE
	for(int k=0; k<argc-1; k++){
		if(rel_arr[k].rel_err){ printf("[EINVALARG]\n"); }
		printf("rel_arr[%d].rel_err = %d\n"
		       "rel_arr[%d].rel_idx = %d\n\n", 
		       k, rel_arr[k].rel_err, k, rel_arr[k].rel_idx);	
	}
	printf("%d unknown arguments found\n", unknown_arg_flag);
#endif

#if LOG_ENABLE == TRUE
	for(int l=0; l<CLI_ARG_COUNT; l++){
		if(ARGS[l].arg_misuse){ printf("[EMISUSEDARG]\n"); }
		printf("ARGS[%d].arg_str = %s\n"
		       "ARGS[%d].arg_str_size = %d\n"
		       "ARGS[%d].arg_fptr = %p\n"
		       "ARGS[%d].arg_reuse = %d\n"
		       "ARGS[%d].arg_reuse_count = %d\n"
		       "ARGS[%d].arg_use_check = %d\n"
		       "ARGS[%d].arg_misuse = %d\n\n",
			l, ARGS[l].arg_str, l, ARGS[l].arg_str_size, l, ARGS[l].arg_fptr, l, ARGS[l].arg_reuse, l, ARGS[l].arg_reuse_count, l, ARGS[l].arg_use_check, l, ARGS[l].arg_misuse);
	}
	printf("%d arguments are misused\n", misuse_arg_flag);
#endif

	if(unknown_arg_flag){ return EINVALARG; }
	if(misuse_arg_flag){ return EARGMISUSED; }
	
	
	LOG_MSG("dispatching...");
	
	// all CLI args parsed, corresponding indices are stored in index_in_cli_arg_table
	for(int m=1;m<argc;m++){
		// no need to check for error cuz assuming the most safe method, no arg must be dispatched if any of the arg causes issue allowing inconsistent program states
		
		if(strlen(argv[m])==ARGS[rel_arr[m-1].rel_idx].arg_str_size){
			// its a switch type CLI ARG, hence no '=' sign there
			if((ARGS[rel_arr[m-1].rel_idx].arg_fptr)(NULL)){ return (m*EHANDLER); }
		}else{
			// only other case is size of user passed arg > declared cli arg size, thus in this case its not a switch type arg
			// size of user passed arg < declared cli arg cuz such cases will be removed during args verification, thus marked as unknow arg 
			if((ARGS[rel_arr[m-1].rel_idx].arg_fptr)((void*)(argv[m] + ARGS[rel_arr[m-1].rel_idx].arg_str_size + 1))){ return (m*EHANDLER); }	
			// passing pointer to char next to '=' which can be then processed by handler
		}
	}
	return 0;		
}

