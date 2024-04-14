#include<reg52.h>
#include<stdio.h>
#include<absacc.h>

#define BAUDRATE 0xF4          //2400 baud rate

void UART_Init(void);
unsigned char UART_Receive();
void UART_Transmit(unsigned char serialdata);
void delay(unsigned int t);



void main()
{
   unsigned char x,y;    
        UART_Init();       
        while(1) 
        {
          int i;
					int counter=0;
					for(i=0;i<1000;i++)           //recieving from pc
					{               
					x=UART_Receive();
						counter += 1;
						XBYTE[i]=x;
					}     
					
					counter=0;
					
					for(i=0;i<1000;i++)           //sending back to pc
					{               
					 y=XBYTE[i];
					 UART_Transmit(y);
						counter +=1;
					}    
					
        }
}

void UART_Init(void)                    // INITIALIZE SERIAL PORT
{
        TMOD = 0x20;                            // Timer 1 IN MODE 2 -AUTO RELOAD TO GENERATE BAUD RATE
        SCON = 0x50;                                      // SERIAL MODE 1, 8-DATA BIT 1-START BIT, 1-STOP BIT, REN ENABLED
        TH1 = BAUDRATE;                        // LOAD BAUDRATE TO TIMER REGISTER
        TR1 = 1;                                    // START TIMER
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