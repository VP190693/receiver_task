#include<reg52.h>

#include"I2C.h"
#include"delay.h"



void UsartConfiguration();


void addSample(unsigned int value);
void print_BR(void);
unsigned char EepromReadByte(unsigned char Address,unsigned char Page);
void EepromWriteByte( unsigned char Data,unsigned char Address, unsigned char Page);


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
     UsartConfiguration();   
	       
        TH0 =	0xA4;
        IE = 0x92;	 
        TR0 = 1;	
       

	while(1) 
        {
         unsigned int i;
					
					for(i=0;i<1024;i++)           //recieving from pc
					{               
					   x=receiveData;
						rate=(counter-counter_prev);
						addSample(rate);
						counter += 1;
						EepromWriteByte(x,i,0);
					}     
					
					counter=0;
					counter_prev=0;
					
					for(i=0;i<1024;i++)           //sending back to pc
					{               
					 y=EepromReadByte(i,0);
					SBUF=y;
						counter +=1;
					}    
					
        }
}


void UsartConfiguration()
{
	SCON=0X50;			
	TMOD=0X22;			
	PCON=0X80;			
	TH1=0XF3;				
	TL1=0XF3;
	ES=1;						
	EA=1;						
	TR1=1;					
}


void addSample(unsigned int value) {
  unsigned int xdata sampleData[1024];
int nextSlot = 0;
 
	
	unsigned int sum = 0;
   int i;
   sampleData[nextSlot] = value;
   nextSlot++;
   if(nextSlot >= 1024) {
		 nextSlot = 0;
	 }
   for( i = 0; i < 1024; i++) 
	{
		sum += sampleData[1024];
  }
	averageBR = sum/1024;
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
		
		           msg[10]=(unsigned char)averageBR;
		               for(i=0;i<15;i++)
		{
				         SBUF=mes[i];
while(!TI);
		}      
		         for(i=0;i<10;i++)
		{
				         SBUF=(msg[i]);
while(!TI);
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
if(TI==1){			 
	  TI=0;
	}		
   else
	 {
	 receiveData=SBUF;
	RI = 0;
	 }
}

