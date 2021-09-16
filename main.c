#include "Can.h"

void SystemInit() {};
char data1[] = "Mohamed1";
volatile char* data2=NULL;
	int main()
{
	CAN_init(0,test_mode);
	CAN_Transmit_Set(0,0x01,Standard,0x05);
	CAN_Receive_Set(0,0x02);
	CAN_Transmit_Data(0,0x01,data1);
	data2=CAN_Receive_Data(0,0x02);
}