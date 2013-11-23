/*
18-342 Lab3
Group-34
Sanjay K Jayasankar - sjayasan
Shobhit Kukreti - skukreti
Yashwant Vemuru - yvemuru
Date: Nov 10, 2013
*/
/*Constant values that are useful for clean implementation of the kernel*/

#define CLR_SWI_NUM 0xff000000
#define BIT_0 0x1

#define SWI_VTABLE_ADDR 0x08
#define IRQ_VTABLE_ADDR 0x18
#define FIQ_VTABLE_ADDR 0x1C

#define UBOOT_SWI_PC 0x24 //(SWI_VTABLE_ADDR+PC_OFFSET+0x14)
#define UBOOT_IRQ_PC 0x34
#define UBOOT_FIQ_PC 0x38


#define UBOOT_SWI 0x5c0009c0
#define UBOOT_IRQ 0x5c000b40
#define UBOOT_FIQ 0x5c000ba0


#define EXEC_NXT_INSTR 0xe51ff004 //op code for => ldr pc,[pc,#-4]
#define PC_OFFSET 0x8
#define NEXT_INSTR 0x4
#define OFFSET_12 0xFFF  //least significant 12-bits of LDR

#define ONE_SEC	1000//ms


/* Parameters that can be modified*/
#define USER_APP_LOCATION 0xa0000000
#define USER_STACK_TOP 0xa3000000
#define IRQ_STACK_SIZE 1024 //number of words
#define TIMER_RES 10 //ms


