#include<reg52.h>
#include<stdio.h>
#include<absacc.h>              //for external memory functions
#include"I2C.h"
#include"delay.h"

#define BAUDRATE 0xF4          //2400 baud rate


unsigned char UART_Receive();
void UART_Transmit(unsigned char serialdata);
void delay(unsigned int t);
void addSample(float value);
void print_BR(void);
unsigned char EepromReadByte(unsigned char Address,unsigned char Page);
void EepromWriteByte( unsigned char Data,unsigned char Address, unsigned char Page);


unsigned int counter=0;
static int counter_prev=0; 
float rate;
double averageBR;

void main()
{
   unsigned char x,y;    
        //UART  Initiation
        
	TMOD = 0x22;                            
        SCON = 0x50;                                      
        TH1 = BAUDRATE; 
        TH0 =	0xA4;
        IE = 0x92;	 
        TR1 = 1;
        TR0 = 1;	
       

	while(1) 
        {
         unsigned int i;
					
					for(i=0;i<1024;i++)           //recieving from pc
					{               
					x=UART_Receive();
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
					 UART_Transmit(y);
						counter +=1;
					}    
					
        }
}

void addSample(float value) {
  double xdata sampleData[1024];
int nextSlot = 0;
 
	
	double sum = 0;
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

void UART_Transmit(unsigned char serialdata)
{
        SBUF = serialdata;                                   // LOAD DATA TO SERIAL BUFFER REGISTER
        while(TI == 0);                                      // WAIT UNTIL TRANSMISSION TO COMPLETE
        TI = 0;                                                    // CLEAR TRANSMISSION INTERRUPT FLAG
}
unsigned char UART_Receive()
{
         while(RI==0);
         RI=0;    
         return SBUF;       
}

void print_BR(void)
	{
	int i;	
		unsigned char msg[10];
  unsigned char mes[15] ="Baud rate = ";		
		
		           sprintf(msg,"%lf",averageBR);
		               for(i=0;i<15;i++)
		{
				           UART_Transmit(mes[i]);

		}      
		          for(i=0;i<10;i++)
		{
				           UART_Transmit(msg[i]);

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

