#include <LPC17xx.H>
void delay(unsigned long int x);
void lcdwrite(unsigned char ch);
unsigned short int  adc,rem1,rem2,rem3,temp;
char arr[5];
int main()
{

unsigned long int i;
unsigned char cmd[] ={0X38,0X33,0X32,0x28,0x0E,0x06,0X01};
unsigned char msg1[]= "Temperature ";
SystemInit();

LPC_GPIO0->FIOMASK=0xE1FFFFFF;// 25-28
LPC_GPIO0->FIODIR=0x1E000000;// 25-28
LPC_GPIO2->FIOMASK1=0xC7;//11,12,13
LPC_GPIO2->FIODIR1=0x38;// rs,rw,EN
LPC_GPIO2->FIOCLR1=0x18;// rs,rw=0

LPC_SC->PCONP|=0X00001000; 
LPC_PINCON->PINSEL1|=0X00004000; //p0.23 as ad0.0   // 0000 0000 0000 0000 0100 0000 0000 0000
LPC_ADC->ADCR=0X00210301; //AD0.0
for(i=0;i<5;i++)
	{
	lcdwrite(cmd[i]);
	}
	
	LPC_GPIO2->FIOCLR1=0x08;//rs=0
  lcdwrite(0X80);
	 delay(50000);
	
	LPC_GPIO2->FIOSET1=0x08;//rs=1
	for(i=0;msg1[i]!='\0';i++)
  {
		lcdwrite(msg1[i]);
   }
	
	
while(1) 		
{                         
while((LPC_ADC->ADSTAT&0X00000001)!=0X00000001)
	{
	}
temp=((LPC_ADC->ADDR0>>4)& 0x00000fff); // shift it and take least 12 bits

LPC_GPIO2->FIOCLR1=0x08;//rs=0
lcdwrite(0XC0);
delay(50000);
	
//	temp=324;
adc=temp/12 ;
LPC_GPIO2->FIOSET1=0x08;//rs=1

rem1=adc % 0X0A ;
adc=adc / 0X0A ;
rem2=adc % 0X0A ;
adc=adc / 0X0A ;
rem3=adc % 0X0A ;
adc=adc / 0X0A ;
	
arr[0]=adc+0X30;
arr[1]=rem3+0X30;
arr[2]=rem2+0X30;
arr[3]=rem1+0X30;
	
for(i=0;i<4;i++)
	{
		lcdwrite(arr[i]);
	}
LPC_GPIO2->FIOCLR1=0x08;	
lcdwrite(0X80);	
delay(0X50000);
//    lcdwrite(0X01);
//	  delay(0X50000);
}
}

void lcdwrite(unsigned char ch)
{
		
    // HIGH nibble
    LPC_GPIO0->FIOPINH = (ch & 0xF0) << 5;  
    LPC_GPIO2->FIOSET1 = 0x20;  // EN = 1
    delay(500000);
    LPC_GPIO2->FIOCLR1 = 0x20;  // EN = 0

    // LOW nibble
    LPC_GPIO0->FIOPINH = (ch & 0x0F) << 9;  
    LPC_GPIO2->FIOSET1 = 0x20;
    delay(500000);
    LPC_GPIO2->FIOCLR1 = 0x20;
	
		delay(500000);
}

void delay(unsigned long int x)
{
	unsigned long int k=0;
	for(k=0;k<x;k++);
}