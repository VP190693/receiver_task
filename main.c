#include<reg52.h>
#include<stdlib.h>
#include"I2C.h"
#include"delay.h"
#include<stdio.h>
#include<math.h>


void UsartConfiguration();


void addSample(unsigned int value);
void print_BR(void);
unsigned char EepromReadByte(unsigned char Address,unsigned char Page);
void EepromWriteByte( unsigned char Data,unsigned char Address, unsigned char Page);
char* itoa(int num, char* buffer, int base);

unsigned int counter=0;
static int counter_prev=0; 
unsigned int rate;
unsigned int averageBR;
unsigned char data_to_send;
	unsigned char receiveData;

void main()
{
   unsigned char x,y;    
        //UART  Initiation
    unsigned int i;
					    
	       UsartConfiguration();
         TH0 =	0xA4;
        IE = 0x92;	 
        TR0 = 1;	
       PT0=0;

					for(i=0;i<1024;i++)           //recieving from pc
					{               
					   
						x=receiveData;
						rate=(counter-counter_prev);
						addSample(rate);
						counter += 1;
						EepromWriteByte(x,i,1);
					  
					}
counter=0;
					counter_prev=0;

						for(i=0;i<1024;i++)           //sending back to pc
					{               
					
						y=EepromReadByte(i,1);
					SBUF=y;
					while(!TI);
		       TI=0;	
						counter +=1;
					}   
					
	while(1) 
        {
         
        }
}


void UsartConfiguration()
{
	SCON=0X50;			
	TMOD=0X20;			
	PCON=0X80;			
	TH1=0XF3;				
	TL1=0XF3;
	ES=1;						
	EA=1;						
	TR1=1;					
}


void addSample(unsigned int value) {
  unsigned int xdata sampleData[50];
int nextSlot = 0;
 
	
	unsigned int sum = 0;
   int i;
   sampleData[nextSlot] = value;
   nextSlot++;
   if(nextSlot >= 50) {
		 nextSlot = 0;
	 }
   for( i = 0; i < 50; i++) 
	{
		sum += sampleData[50];
  }
	averageBR = sum/50;
}

void timer0() interrupt 1
{
  
	 static int count=0;	
   
	if(count>10000)                   //baud rate calculation every second
	{
    
		print_BR();	 
		
		counter_prev=counter;
		count=0;
	}
	else
	{
  count++;	
	}
}


void print_BR(void)
	{
	
		int i;	
		unsigned char msg[10];
  unsigned char mes[15] ="Baud rate = ";		
		
		           itoa(averageBR,msg,10);
UsartConfiguration();		 
		for(i=0;i<15;i++)
		{
				         SBUF=mes[i];
while(!TI);
			TI=0;
		}      
		         for(i=0;i<10;i++)
		{
				         SBUF=(msg[i]);
while(!TI);
		TI=0;
		}
	
		
	}

void EepromWriteByte( unsigned char Data,unsigned char Address, unsigned char Page)
{

    I2CStart(); 
   	I2CSend(0xA0|(Page<<1));	
   	I2CSend(Address); 
   	I2CSend(Data);    
    I2CStop();           	 
	  Delay_ms(5);        
}



unsigned char EepromReadByte(unsigned char Address,unsigned char Page)
{
  unsigned char Data;

    I2CStart();            
   	I2CSend(0xA0|(Page<<1));	  
   	I2CSend(Address); 
    I2CStart();		   
    I2CSend(0xA1|(Page<<1));           
	  Data = I2CRead(); 
	  I2CNak();
    I2CStop();		        
	  Delay_us(10);
    return Data;          
}

void Usart() interrupt 4
{
 	
 
	receiveData=SBUF;
	 RI = 0;
	 
	 //SBUF='*';
	      
		}

char* itoa(int num, char* buffer, int base)   
{  
int current = 0;  
unsigned int num_digits = 0;
	if (num == 0) {  
buffer[current++] = '0';  
buffer[current] = '\0';  
return buffer;  
}  
  
if (num < 0) {  
if (base == 10) {  
num_digits ++;  
buffer[current] = '-';  
current ++;  
num *= -1;  
}  
else  
return NULL;  
}  
num_digits += (int)floor(log(num) / log(base)) + 1;  
while (current < num_digits)   
{  
int base_val = (int) pow(base, num_digits-1-current);  
int num_val = num / base_val;  
 char value = num_val + '0';  
buffer[current] = value;  
current ++;  
num -= base_val * num_val;  
}  
buffer[current] = '\0';  
return buffer;  
}  