/*
 * System call stub for minilib C library
 * GPL licensed code (S. Di Mercurio and P. Acco)
 * _________________
 * USAGE 
 *  copy this minilib/syscalls_template/syscalls.c in the project dir as syscalls.c
 * 	add path to minilib/include in your compiler include path
 *  add #include "missing_defs.h" in order to have struct FILE declared
 *  modify your copy of syscalls.c to have it work as you like (current function implementations are just stubs)
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "missing_defs.h"

//TODO blabla
char minilib_read(int);
char minilib_write(int, char);


#ifdef __GNUC__
#if !defined (__AVR__)
	#include <sys/stat.h>
	#include <sys/types.h>

#else
	//#include <sys/types.h>
	#define ENOMEM -2
	#define EAGAIN -3
	#define EINVAL -4
	#define EMLINK -5
	#define ENOENT -6
	#define ECHILD -7

#endif /* !defined __AVR__ */
#else
	#define ENOMEM -2
	#define EAGAIN -3
	#define EINVAL -4
	#define EMLINK -5
	#define ENOENT -6
	#define ECHILD -7
	
	#define S_IFCHR 0x12
	#define caddr_t int	
	
	struct stat
	{
		int st_mode;	
	}; 

	struct tms
	{
		int a;
	};
#endif /* __GNUC__ */

#undef errno
int errno;
	   
/*
 * _exit
 *   Exit a program without cleaning up files. If your system doesn't provide this, it is best to avoid linking with subroutines that require it (exit, system).
 */
void _exit(int exit_code)
{
	/* Nothing to do */
	for (;;);
}

/*
 * _kill
 * Send a signal. Minimal implementation:
 */
int _kill(int pid, int sig) 
{
	errno = EINVAL;
	return -1;
}

/*
 * _open
 * Open a file. Minimal implementation:
 */
int _open(const char *name, int flags, int mode) 
{
	return -1;
}

/*
 * _read
 * Read from a file. 
 * 	file : file identifier (indicate on which channel/peripheral read data)
 * 	ptr : pointer to an array of char (provided by caller) in which store data
 *	len : amount of data to read
 */
int _read(int file, char *ptr, int len) 
{
char *loc_ptr=ptr;

	while (loc_ptr-ptr < len) 
	{
		*loc_ptr = minilib_read(file);
		loc_ptr++;
	}	
	
	return len;
}

/*
 *	_write
 *  Write to a file. 
 * 	file : file identifier (indicate in which channel/peripheral write data)
 * 	ptr : pointer to an array of char (provided by caller) on which take data to write
 *	len : amount of data to write
 */
int _write(int file, char *ptr, int len) 
{
char *loc_ptr=ptr;

	while (loc_ptr-ptr < len) 
	{
		minilib_write(file,*loc_ptr);
		loc_ptr++;
	}	
	
	return len;
}

/*
 * _malloc
 * Allocate memory. Minimal implementation:
 */
void *_malloc(size_t nbytes) 
{
	return NULL;
}

/*
 * _free
 * Release memory. Minimal implementation:
 */
void _free(void *aptr) 
{
}

/*****************************************************************************************************
 * Functions used when GCC use builtin functions, that you need to link with libc and that minilib has not yet redefined the function
 *****************************************************************************************************/
/*caddr_t _sbrk(int incr) 
{
	extern char _end; 
	static char *heap_end;
	char *prev_heap_end;
	static char *stack_ptr;
	
	if (heap_end == 0) 
	{
		heap_end = &_end;
	}

	prev_heap_end = heap_end;

	if (heap_end + incr > stack_ptr) 
	{
		_write (1, "Heap and stack collision\n", 25);
		//abort ();
	}

	heap_end += incr;
	return (caddr_t) prev_heap_end;
}

int _close(int file) 
{
	return -1;
}

#include <sys/stat.h>
int _fstat(int file, struct stat *st) 
{
	st->st_mode = S_IFCHR;
	return 0;
}

int _isatty(int file) 
{
	return 1;
}

int _lseek(int file, int ptr, int dir) 
{
	return 0;
}

int _getpid(void) 
{
	return 1;
}
*/