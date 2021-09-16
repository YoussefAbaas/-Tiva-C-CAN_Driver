#ifndef CAN_H
#define CAN_H
#include "stdint.h"
#include <stdio.h>
#include "tm4c123gh6pm.h"
/*Test Modes*/
#define test_mode   0
#define normal_mode 1

/*Identifier types*/
#define Extended 0
#define Standard 1

/*function prototypes*/
void  CAN_init(unsigned char Can_id,char mode);
void  CAN_Transmit_Set(unsigned char Can_id,int objNo,unsigned char identifier_type,int msg_id);
void  CAN_Receive_Set(unsigned char Can_id,int objNO);
void  CAN_Transmit_Data(unsigned char Can_id,int objNo,char data[8]);
char* CAN_Receive_Data(unsigned char Can_id,int objNo);

#endif