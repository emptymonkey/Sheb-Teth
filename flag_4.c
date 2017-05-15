
#include "common.h"
#include "libxorscura.h"
#include "libptrace_do.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int check_flag_4(char *query){

	//	char *flag_4_plaintext = "7d394dc8-38da-11e7-8cb8-1f6f0a52a62f";
	//	unsigned int flag_4_seed = 1204032042;
	//	unsigned char flag_4_key[] = {0xe4,0x03,0x0e,0x0a,0x18,0x2c,0x5a,0x18,0x80,0xfc,0x35,0x6f,0xca,0x00,0x8f,0x20,0x7f,0xf4,0x47,0x36,0x0b,0xc7,0xaf,0x60,0x90,0x63,0x73,0x2f,0x09,0x1a,0xf2,0x55,0x79,0x23,0x74,0x3d};

	unsigned char flag_4_ciphertext[] = {0xd3,0x67,0x3d,0x33,0x2c,0x48,0x39,0x20,0xad,0xcf,0x0d,0x0b,0xab,0x2d,0xbe,0x11,0x1a,0xc3,0x6a,0x0e,0x68,0xa5,0x97,0x4d,0xa1,0x05,0x45,0x49,0x39,0x7b,0xc7,0x67,0x18,0x15,0x46,0x5b};

	//	unsigned char *hidden_location_plaintext = "/proc/self/exe";
	unsigned int hidden_location_seed = 443823568;
	unsigned char hidden_location_ciphertext[] = {0xab,0xeb,0x77,0x6f,0xe9,0xd5,0x97,0x2a,0x3c,0xed,0x7a,0x0c,0xf6,0xa8};
	unsigned int hidden_buff_len = 14;

	int retval;
	int fd;
	int match = 0;
  struct xod *xor_data;
	int gppid, ppid, pid, cpid, sid;

	struct ptrace_do *spawn;

	// We will do ptrace operations on this variable. Thus, word aligned for ease.
	unsigned char *tmp_ptr __attribute__ ((aligned (__WORDSIZE)));


	gppid = getppid();

	if((xor_data = (struct xod *) calloc(1, sizeof(struct xod))) == NULL){
		error(-1, errno, "calloc(1, %d)", (int) sizeof(struct xod));
	}


	/*
		 In this scenario, the debugging will be frustrated because there is a child process that
		 will be assisting us. IPC with the child will take place through ptrace. Further, the
		 child will be responsible for the anti-debug check. If the child's grandparent isn't the 
		 session leader, then we aren't being run out of a shell. We will assume we are being 
		 debugged and kill the grandparent, parent, and then ourselves.

		 Finally, the child will extract the key from dead space in our own elf binary. The parent
		 will retreave the key with ptrace.
	 */

	cpid = fork();

	if(!cpid){

		ptrace(PTRACE_TRACEME, 0, 0, 0);
		__asm__(
				"int3 \n\t"
				);

		sid = getsid(0);
		ppid = getppid();
		pid = getpid();

		if(sid != gppid){
			kill(gppid, SIGKILL);
			kill(ppid, SIGKILL);
			kill(pid, SIGKILL);
		}

		xor_data->buf_count = hidden_buff_len;
		xor_data->seed = hidden_location_seed;
		xor_data->ciphertext_buf = hidden_location_ciphertext;

		if(xorscura_decrypt(xor_data) == -1){
			error(-1, errno, "xorscura_decrypt(%lx)", (unsigned long) xor_data);
		}


		fd = open((char *) xor_data->plaintext_buf, O_RDONLY);
		lseek(fd, -BUFFER_LEN, SEEK_END);
		read(fd, tmp_ptr, BUFFER_LEN);
		close(fd);

		__asm__(
				"int3 \n\t"
				);


		exit(0);
	}

	wait(NULL);

	spawn = ptrace_do_init(cpid);
	tmp_ptr = (unsigned char *) ptrace_do_malloc(spawn, BUFFER_LEN + 1);
	ptrace(PTRACE_POKEDATA, cpid, &tmp_ptr, ptrace_do_get_remote_addr(spawn, tmp_ptr));
	ptrace(PTRACE_CONT, cpid, 0, 0);
	wait(NULL);
	ptrace_do_pull_mem(spawn, tmp_ptr);
	ptrace(PTRACE_DETACH, cpid, 0, 0);
	wait(NULL);

	xor_data->buf_count = BUFFER_LEN;
	xor_data->key_buf = tmp_ptr;
	xor_data->ciphertext_buf = flag_4_ciphertext;
	xor_data->plaintext_buf = (unsigned char *) query;

	if((retval = xorscura_compare(xor_data)) == -1){
		error(-1, errno, "xorscura_compare(%lx)", (unsigned long) xor_data);
	}

	if(!retval){
		match = 1;
	}

	xorscura_free_xod(xor_data);
	free(xor_data);
	ptrace_do_free(spawn, tmp_ptr, FREE_LOCAL);
	free(spawn);

	return(match);
}
