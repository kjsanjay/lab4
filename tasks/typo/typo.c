/*
18-342 Lab3
Group-34
Sanjay K Jayasankar - sjayasan
Shobhit Kukreti - skukreti
Yashwant Vemuru - yvemuru
Date: Nov 10, 2013
*/

/** @file typo.c
*
* @brief Echos characters back with timing data.

    Prompts user to type characters. 
    The program echoes what has been typed.
    Print out the time it took for the user to type the line
    Program must present the prompt again & never terminates
*
* Links to libc.
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <bits/fileno.h>

#define SIZE 101


int main(int argc, char** argv) {
    
    char buf[SIZE];
    unsigned long start_time = 0;
    unsigned long end_time = 0;
    int num_bytes = 0;
    unsigned long diff;
    
    while (1) {
        memset(buf, 0, SIZE);
        printf("> ");
        start_time = time();
//        printf("%lu\n",start_time);
        num_bytes = read(STDIN_FILENO, buf, SIZE - 1);
        if(buf[num_bytes-1]=='\r') 
        {
        //if enter is pressed 
            buf[num_bytes-1]='\0';
        }
        else // If size limit is reach
            printf("\n");

        end_time = time();
  //              printf("%lu\n",end_time);
        diff = end_time - start_time;
        printf("%lu.%lus\n", diff / 1000, (diff % 1000) / 100);
    }
    return 0;
}

