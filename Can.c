#include "Can.h"


void CAN_init(unsigned char Can_id,char mode)
{
	if(Can_id==0)
	{
	volatile unsigned long delay;
SYSCTL_RCGC0_R|=(1<<24); //enable clock for Can0
SYSCTL_RCGC2_R|=(1<<1); //enable clock for portB
delay=SYSCTL_RCGC2_R;
GPIO_PORTB_LOCK_R= 0x4C4F434B;
GPIO_PORTB_CR_R |= 0x30;
GPIO_PORTB_AFSEL_R |= 0x30;
GPIO_PORTB_DEN_R |= 0x30;
GPIO_PORTB_DIR_R |= 0x20; // pin 4 is input and pin 5 is output
GPIO_PORTB_PCTL_R=(GPIO_PORTB_PCTL_R&0xff00ffff)|(0x00880000); // can 0 function for bit 4 and 5 
while(CAN0_IF1CRQ_R & 0x00008000);//wait while CAN0 Module is BUSY
CAN0_CTL_R|=(1<<0)|(1<<6);//set init and CCE bit in CANCTL register;

if(mode == test_mode)
{
	  CAN0_CTL_R |= (1<<7);  //enable test mode 
    CAN0_TST_R |= (1<<5); //enable loopback mode
}
else if(mode == normal_mode)
{
	  CAN0_CTL_R &=~ (1<<7);  //enable test mode 
    CAN0_TST_R &=~ (1<<5); //enable loopback mode
}
CAN0_BIT_R|=0x204; // bit timing ex1 in data sheet
CAN0_CTL_R&=~(1<<0); //can is enabled (init=0)
}
	else if(Can_id==1)
	{
	volatile unsigned long delay;
SYSCTL_RCGC0_R|=(1<<25); //enable clock for Can1
SYSCTL_RCGC2_R|=(1<<0); //enable clock for portA
delay=SYSCTL_RCGC2_R;
GPIO_PORTA_LOCK_R= 0x4C4F434B;
GPIO_PORTA_CR_R |= 0x03;
GPIO_PORTA_AFSEL_R |= 0x03;
GPIO_PORTA_DEN_R |= 0x03;
GPIO_PORTA_DIR_R |= 0x02; // pin 0 is input and pin 1 is output
GPIO_PORTA_PCTL_R=(GPIO_PORTA_PCTL_R&0xffffff00)|(0x00000088); // can 1 function for bit 0 and 1
while(CAN1_IF1CRQ_R & 0x00008000);//wait while CAN1 Module is BUSY
CAN1_CTL_R|=(1<<0)|(1<<6);//set init and CCE bit in CANCTL register;

if(mode == test_mode)
{
	  CAN1_CTL_R |= (1<<7);  //enable test mode 
    CAN1_TST_R |= (1<<5); //enable loopback mode
}
else if(mode == normal_mode)
{
	  CAN1_CTL_R &=~ (1<<7);  //enable test mode 
    CAN1_TST_R &=~ (1<<5); //enable loopback mode
}
CAN1_BIT_R|=0x204; // bit timing ex1 in data sheet
CAN1_CTL_R&=~(1<<0); //can is enabled (init=0)
	
	}
}

//Initialise the transmit message object
void CAN_Transmit_Set(unsigned char Can_id,int objNo,unsigned char identifier_type,int msg_id)
{
	if(Can_id==0)
	{
	while(CAN0_IF1CRQ_R & 0x00008000);//wait while CAN0 Module is BUSY

	CAN0_IF1CMSK_R|=(1<<7)|(1<<5)|(1<<4)|(1<<0)|(1<<1);//ARB, WRNRD, CONTROL, DATAA, DATAB bits set
	CAN0_IF1MCTL_R|=(1<<12); //UMASK bit sets
	CAN0_IF1ARB2_R|=(1<<13); //set dir bit
	//CAN0_IF1ARB2_R|=(1<<2); //MSG ID set to 0x01 in ARB2 reg
	//CAN0_IF1ARB2_R&=~(1<<14); //clear xtd bit to indicate a standard identifier
	CAN0_IF1ARB2_R|=(1<<15);//set Msgval bit to indicate that message object is valid
	
	//data to be sent
	  CAN0_IF1DA1_R = 0x5453; //Actual Data to be sent &quot;START&quot; (Not required here, can be ignored)
    CAN0_IF1DA2_R = 0x5241; //This is the first message sent.
    CAN0_IF1DB1_R = 0x0054;
    CAN0_IF1DB2_R = 0x0000;
		
		if(identifier_type==Extended) // 29 bit id
		{
		  CAN0_IF1ARB2_R|=(1<<14); //set xtd bit to indicate an Entended identifier
			CAN0_IF1ARB1_R|=(msg_id&0xffff); //configure ID[15:0] in the CANIFnARB1 register for bits [15:0] of the message identifier
			CAN0_IF1ARB1_R|=(msg_id>>16); // ID[12:0] in the CANIFnARB2 register for bits [28:16] of the message identifier
		}
		else if(identifier_type==Standard)//11 bit id
		{
		  CAN0_IF1ARB2_R&=~(1<<14); //clear xtd bit to indicate a standard identifier
			CAN0_IF1ARB2_R|=(msg_id<<2); //MSG ID set to be in ARB2 reg
		}
	
	  CAN0_IF1CRQ_R=objNo; //set message number in mnum bit
	  CAN0_IF1MCTL_R|=(1<<8); //set TXRQST bit so bit is set, the message object is available to be transmitted, depending on priority and bus availability
	}
	else if(Can_id==1)
	{
	while(CAN1_IF1CRQ_R & 0x00008000);//wait while CAN1 Module is BUSY

	CAN1_IF1CMSK_R|=(1<<7)|(1<<5)|(1<<4)|(1<<0)|(1<<1);//ARB, WRNRD, CONTROL, DATAA, DATAB bits set
	CAN1_IF1MCTL_R|=(1<<12); //UMASK bit sets
	CAN1_IF1ARB2_R|=(1<<13); //set dir bit
	//CAN1_IF1ARB2_R|=(1<<2); //MSG ID set to 0x01 in ARB2 reg
	//CAN1_IF1ARB2_R&=~(1<<14); //clear xtd bit to indicate a standard identifier
	CAN1_IF1ARB2_R|=(1<<15);//set Msgval bit to indicate that message object is valid
	
	//data to be sent
	  CAN1_IF1DA1_R = 0x5453; //Actual Data to be sent &quot;START&quot; (Not required here, can be ignored)
    CAN1_IF1DA2_R = 0x5241; //This is the first message sent.
    CAN1_IF1DB1_R = 0x0054;
    CAN1_IF1DB2_R = 0x0000;
		
	if(identifier_type==Extended) // 29 bit id
		{
		  CAN1_IF1ARB2_R|=(1<<14); //set xtd bit to indicate an Entended identifier
			CAN1_IF1ARB1_R|=(msg_id&0xffff); //configure ID[15:0] in the CANIFnARB1 register for bits [15:0] of the message identifier
			CAN1_IF1ARB1_R|=(msg_id>>16); // ID[12:0] in the CANIFnARB2 register for bits [28:16] of the message identifier
		}
		else if(identifier_type==Standard)//11 bit id
		{
		  CAN1_IF1ARB2_R&=~(1<<14); //clear xtd bit to indicate a standard identifier
			CAN1_IF1ARB2_R|=(msg_id<<2); //MSG ID set to be in ARB2 reg
		}
	  CAN1_IF1CRQ_R=objNo; //set message number in mnum bit
	  CAN1_IF1MCTL_R|=(1<<8); //set TXRQST bit so bit is set, the message object is available to be transmitted, depending on priority and bus availability
	}
	
	}
	
//Initialise the receive message object
void CAN_Receive_Set(unsigned char Can_id,int objNO)
{
	if(Can_id==0)
	{
	CAN0_IF2CMSK_R|=0x000000f3; //WRNRD, CONTROL, MASK, DATAA, DATAB,ARB bits set
	CAN0_IF2MCTL_R|=(1<<12); //set UMASK bit
	CAN0_IF2ARB2_R|=(1<<15);//set Msgval bit to indicate that message object is valid
	CAN0_IF2MCTL_R|=(1<<7); //set EOB bit for single message object 
	CAN0_IF2MCTL_R|=0x0008;//set dlc bits with 8 bytes 
	
	CAN0_IF2CRQ_R=objNO; //set message number in mnum bit
	while(CAN0_IF2CRQ_R & 0x00008000);//wait while CAN0 Module is BUSY
	}
	else if(Can_id==1)
	{
	CAN1_IF2CMSK_R|=0x000000f3; //WRNRD, CONTROL, MASK, DATAA, DATAB,ARB bits set
	CAN1_IF2MCTL_R|=(1<<12); //set UMASK bit
	CAN1_IF2ARB2_R|=(1<<15);//set Msgval bit to indicate that message object is valid
	CAN1_IF2MCTL_R|=(1<<7); //set EOB bit for single message object 
	CAN1_IF2MCTL_R|=0x0008;//set dlc bits with 8 bytes 
	
	CAN1_IF2CRQ_R=objNO; //set message number in mnum bit
	while(CAN1_IF2CRQ_R & 0x00008000);//wait while CAN1 Module is BUSY
	}
}

void CAN_Transmit_Data(unsigned char Can_id,int objNo,char data[8])
{
	if(Can_id==0)
	{
	CAN0_IF1CMSK_R&=(~(1<<4))&(~(1<<5));//clear control and Arb bit
	CAN0_IF1CMSK_R|=(1<<0)|(1<<1)|(1<<7); //enable WRND,DATAA,DATAB bits
	
	  CAN0_IF1DA1_R = ((data[1]<<8)+data[0]); //Actual Data
    CAN0_IF1DA2_R = ((data[3]<<8)+data[2]);
    CAN0_IF1DB1_R = ((data[5]<<8)+data[4]);
    CAN0_IF1DB2_R = ((data[7]<<8)+data[6]);
	
	CAN0_IF1CRQ_R=objNo; //set message number in mnum bit
	CAN0_IF1MCTL_R|=(1<<8); //set TXRQST bit
	
	while(CAN0_IF2CRQ_R & 0x00008000);//wait while transmission in done
	}
	else if(Can_id==1)
	{
	CAN1_IF1CMSK_R&=(~(1<<4))&(~(1<<5));//clear control and Arb bit
	CAN1_IF1CMSK_R|=(1<<0)|(1<<1)|(1<<7); //enable WRND,DATAA,DATAB bits
	
	  CAN1_IF1DA1_R = ((data[1]<<8)+data[0]); //Actual Data
    CAN1_IF1DA2_R = ((data[3]<<8)+data[2]);
    CAN1_IF1DB1_R = ((data[5]<<8)+data[4]);
    CAN1_IF1DB2_R = ((data[7]<<8)+data[6]);
	
	CAN1_IF1CRQ_R=objNo; //set message number in mnum bit
	CAN1_IF1MCTL_R|=(1<<8); //set TXRQST bit
	
	while(CAN1_IF2CRQ_R & 0x00008000);//wait while transmission in done
	}
}
char data[8] = {0};
char* CAN_Receive_Data(unsigned char Can_id,int objNo)
{
	if(Can_id==0)
	{
	while(CAN0_IF2CRQ_R & 0x00008000);//wait while transmission in done
	CAN0_IF2CMSK_R|=0x00000073; //CONTROL, MASK, DATAA, DATAB,ARB bits set
	CAN0_IF2CRQ_R=objNo; //set message number in mnum bit to transfer data to interface registers
	
	if(CAN0_IF2MCTL_R&(1<<15))//check if new dat bit is set , so there is new data received in registers 
	{
		data[0] = (0x00FF & CAN0_IF2DA1_R); //Get this new data.
        data[1] = ((0xFF00 & CAN0_IF2DA1_R)>>8);
        data[2] = (0x00FF & CAN0_IF2DA2_R);
        data[3] = ((0xFF00 & CAN0_IF2DA2_R)>>8);
        data[4] = (0x00FF & CAN0_IF2DB1_R);
        data[5] = ((0xFF00 & CAN0_IF2DB1_R)>>8);
        data[6] = (0x00FF & CAN0_IF2DB2_R);
        data[7] = ((0xFF00 & CAN0_IF2DB2_R)>>8);
	}
	if(CAN0_IF2MCTL_R&(1<<14))//check if msglst bit is set to check for message los
	{
		/* Call handler to handle if more than one message has been received since the last time this message object was read  */
		CAN0_IF2MCTL_R&=~(1<<14); //reset msglst bit 
	}
}
	else if(Can_id==1)
	{
	while(CAN1_IF2CRQ_R & 0x00008000);//wait while transmission in done
	CAN1_IF2CMSK_R|=0x00000073; //CONTROL, MASK, DATAA, DATAB,ARB bits set
	CAN1_IF2CRQ_R=objNo; //set message number in mnum bit to transfer data to interface registers
	
	if(CAN1_IF2MCTL_R&(1<<15))//check if new dat bit is set , so there is new data received in registers 
	{
		data[0] = (0x00FF & CAN1_IF2DA1_R); //Get this new data.
        data[1] = ((0xFF00 & CAN1_IF2DA1_R)>>8);
        data[2] = (0x00FF & CAN1_IF2DA2_R);
        data[3] = ((0xFF00 & CAN1_IF2DA2_R)>>8);
        data[4] = (0x00FF & CAN1_IF2DB1_R);
        data[5] = ((0xFF00 & CAN1_IF2DB1_R)>>8);
        data[6] = (0x00FF & CAN1_IF2DB2_R);
        data[7] = ((0xFF00 & CAN1_IF2DB2_R)>>8);
	}
	if(CAN1_IF2MCTL_R&(1<<14))//check if msglst bit is set to check for message los
	{
		/* Call handler to handle if more than one message has been received since the last time this message object was read  */
		CAN1_IF2MCTL_R&=~(1<<14); //reset msglst bit 
	}
	}
	return data;
}