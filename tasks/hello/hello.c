/** @file hello.c
 *
 * @brief Prints out Hello world using the syscall interface.
 *
 * Links to libc.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-10-29
 */
#include <unistd.h>
#include <stdio.h>

const char hello[] = "Hello World\r\n";
char str[40];
//char str1[40];
//int main(int argc, char** argv)
int main(void)
{
    unsigned int a;
    a=read(STDIN_FILENO, str, 20);
//    printf("\nret2=%lu\n",a);
   // printf("\nret3=%d\n",sizeof(str));
    write(STDOUT_FILENO, str, a);
//    printf("\njj");
	//write(STDOUT_FILENO, hello, sizeof(hello) - 1);
	return 0;
}
