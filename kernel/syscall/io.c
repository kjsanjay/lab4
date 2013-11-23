/** @file io.c
 *
 * @brief Kernel I/O syscall implementations
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-16
 */

#include <types.h>
#include <bits/errno.h>
#include <bits/fileno.h>
#include <arm/physmem.h>
#include <syscall.h>
#include <exports.h>
#include <kernel.h>

#define EOT_CHAR 0x04
#define DEL_CHAR 0x7f
// Special Values
#define EOT 				4
#define BACKSPACE 			8
#define DELETE 				127
#define NEW_LINE 			10
#define CARRIAGE_RETURN		13


/* Read count bytes (or less) from fd into the buffer buf. */
ssize_t read_syscall(int fd __attribute__((unused)), void *buf __attribute__((unused)), size_t count __attribute__((unused)))
{

	unsigned int i;
	char* buf_read=(char*)buf;
	if(fd != STDIN_FILENO)
	{
		return -EBADF;
	}
	else if( ( (unsigned int)buf_read <= 0xa0000000) || 
		( ( (unsigned int)buf_read + count) >= 0xa3efffff))
	{
		
		return -EFAULT;
	}
	else
	{
		for(i = 0; i < count; i++)
		{
			char c = getc();
			
			if(c==BACKSPACE || c==DELETE)
			{
				//Backspace & Delete
				if(i>0)
				{
					buf_read[--i]='\0';
					
					puts("\b");
					
				}
				i--;
			}
			else if(c==CARRIAGE_RETURN || c==NEW_LINE)
			{

				buf_read[i]=c;
				// putc('E');
				putc('\n');
				i++;
				break;

			}
			else if(c=='\0' || c==EOT)
			{
				//NULL or EOT
				break;

			}
			else
			{
				buf_read[i] = c;
				putc(buf_read[i]);
			}

				
		}
		
		
	}
	//Number of bytes read
	return i;
	
}

/* Write count bytes to fd from the buffer buf. */
ssize_t write_syscall(int fd  __attribute__((unused)), const void *buf  __attribute__((unused)), size_t count  __attribute__((unused)))
{

	unsigned int i;
	char* buf_write=(char*)buf;
	//Invalid descriptor
	if(fd != STDOUT_FILENO)
	{
		return -EBADF;
	}
	else if( ( (unsigned int)buf_write <= 0xa0000000) || 
		( ( (unsigned int)buf_write + count) >= 0xa3efffff)	)
	{
		
		return -EFAULT;
	}
	else
	{
		for(i = 0; i < count; i++)
		{
			if(buf_write[i]!='\0')
			{
				putc(buf_write[i]);
				if(buf_write[i]=='\n')
				{
					i++;
					break;
				}

			}
			


		}
		
	}	
  
  	//putc('\n');
  	//Number of bytes written
	return i;
	
}

