/*
	Joseph Bieselin
	N 16590510
	jab975

	HW 2:
	Filesystem
*/

#include <fuse.h>
#include <stdlib.h>	// utility functions: atoi, atol
#include <stdio.h>	// printf, fprintf, fgets, fputs
#include <string.h>	// strcpy, strcmp, strncmp, strtok, strlen
#include <assert.h>	// debugging: assert

// DEFINED CONSTANTS TO BE USED (these values may be changed in the future)
#define MAX_NUM_BLOCKS 10000
#define MAX_FILE_SIZE 1638400
#define BLOCK_SIZE 4096
#define CHAR_FILE_LENGTH 50	// the length in characters of the filesystem in the host directory; i.e.: /fuse/fusedata.X --> 0 <= X <= MAX_NUM_BLOCKS - 1

// Default path files
static const char *files_path = "/fuse";

// struct and define format below created from docs from the following URL: http://www.cs.nmsu.edu/~pfeiffer/fuse-tutorial/
struct jab_state {
	char *rootdir;
};
#define JAB_DATA ((struct jab_state *) fuse_get_context()->private_data)
// JAB_DATA can now point to fuse_get_context()'s private_data field which is private filesystem data used while the filesystem is open


// -------------------------- FILESYSTEM FUNCTIONS -------------------------- //

// fuse_conn_info contains connection info that is passed to the ->init() method
// initialize fusedata blocks from fusedata.0 to fusedata.(MAX_NUM_BLOCKS-1)
// Each block is preallocated (upon FS creation) with all zeros and stored on the host file system
void *jab_init(struct fuse_conn_info *conn)
{
	char *path = calloc(1, CHAR_FILE_LENGTH * sizeof(char));
	char i_str[20]; // used for itoa in the for loop below
	char fusedata_str[10]; // will contain "fusedata."
	strcpy(fusedata_str, "fusedata.");
	int i;
	for(i = 0; i < MAX_NUM_BLOCKS; ++i) {
		itoa(i, i_str, 10); // convert i to a string stored in i_str
		strcpy(path, files_path); // re-initialize FUSE root path
		strcat(path, strcat(fusedata_str, i_str)); // path will now look like: /fuse/fusedata.X --> /fuse can be changed by changing files_path and X is an integer value
		FILE *fd = fopen(path, "r");
		if(!fd) {
			fprintf(stderr, "ERROR: could not open fusedata blocks");
			abort();
		}
		fclose(fd);
	}
	free(path); // free up allocated memory space
	return JAB_DATA;
}

// -------------------------- FILESYSTEM FUNCTIONS -------------------------- //


// redefined functions that will call FUSE functions to implement UNIX commands
/*
At a minimum, you must support the following functions:
create
destroy
getattr
init
link
mkdir
open
opendir
read
readdir
readlink
release
releasedir
rename
statfs
unlink
write
*/

static struct fuse_operations jab_oper = {
	.init = jab_init,
};


// Expected arguments (in specified order): jabFS mountPoint
int main(int argc, char *argv[])
{
	// struct and testing created from following URL: http://www.cs.nmsu.edu/~pfeiffer/fuse-tutorial/
	struct jab_state *jab_data;
	jab_data = malloc(sizeof(struct jab_state));
	if(jab_data == NULL) {
		fprintf(stderr, "ERROR: Allocation error in main");
		abort();
	}

	// return 0 on a successful call
	// jabFS_oper will define the functions created for this filesystem
	// NULL cab be user data that will be supplied in the context during init()
	return fuse_main(argc, argv, &jab_oper);
}
