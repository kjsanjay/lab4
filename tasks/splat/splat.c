/*
18-342 Lab3
Group-34
Sanjay K Jayasankar - sjayasan
Shobhit Kukreti - skukreti
Yashwant Vemuru - yvemuru
Date: Nov 10, 2013
*/


/** @file splat.c
 *
 * @brief Displays a spinning cursor.
 *
 	The program must display a spinning cursor.
 	Transition between states every 200 ms
 	Program should never terminate & does not read user input.
 * Links to libc.
 */
 #include <stdio.h>
 #include <unistd.h>

 #define INTERVAL 200

int main(int argc, char** argv)
{
	while(1){
		printf("|");
		sleep(INTERVAL);
		printf("\b");
		printf("/");
		sleep(INTERVAL);
		printf("\b");
		printf("-");
		sleep(INTERVAL);
		printf("\b");
		printf("\\");
		sleep(INTERVAL);
		printf("\b");

	}	

	return 0;
}
