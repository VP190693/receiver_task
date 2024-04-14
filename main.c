#include<reg52.h>
#include<stdio.h>
#include<absacc.h>              //for external memory functions

#define BAUDRATE 0xF4          //2400 baud rate


unsigned char UART_Receive();
void UART_Transmit(unsigned char serialdata);
void delay(unsigned int t);
void addSample(float value);
	
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
          int i;
					
					for(i=0;i<1000;i++)           //recieving from pc
					{               
					x=UART_Receive();
						rate=(counter-counter_prev);
						addSample(rate);
						counter += 1;
						XBYTE[i]=x;
					}     
					
					counter=0;
					counter_prev=0;
					
					for(i=0;i<1000;i++)           //sending back to pc
					{               
					 y=XBYTE[i];
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
    
		printf("BR = %lf",averageBR);	 
		
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

