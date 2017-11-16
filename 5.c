#include <mega128.h>
#include <alcd.h>
#include <stdio.h>
#include <delay.h>
#include <math.h>

#define LU PORTE.0
#define LD PORTE.1
#define RD PORTE.2
#define RU PORTE.3
char toLCD[16];
int decode[9]={0x09,0x08,0x0C,0x04,0x06,0x02,0x03,0x01,0x09};
int nowPosition=0;
int needPosition=0;
int direction=0;//0-right,1-left
int k=0;
int i=0;

int step(int steps,int now,int napr)
{
    switch(napr)
        {
        case 0:
            for(i=0;i<steps;i++)
            {
                for(k=0;k<9;k++){PORTE=decode[k];delay_ms(20);}//right step 
                now+=5;
                sprintf(toLCD,"Now %i grad ",now);
                lcd_gotoxy(0,0);
                lcd_puts(toLCD);
            }
            return now;
            break;  
        case 1:
            for(i=0;i<steps;i++)
            {
                for(k=9;k>=0;k--){PORTE=decode[k];delay_ms(20);}//left step 
                now-=5;
                sprintf(toLCD,"Now %i grad ",now);
                lcd_gotoxy(0,0);
                lcd_puts(toLCD);
            }
            return now;
            break;
        }
}

interrupt [EXT_INT6] void ext_int6_isr(void)
{
    direction=0;
    needPosition+=325;
    sprintf(toLCD,"Need %i grad  ",needPosition);
    lcd_gotoxy(0,1);
    lcd_puts(toLCD);  
}

interrupt [EXT_INT7] void ext_int7_isr(void)
{
    direction=1;
    needPosition-=15;
    sprintf(toLCD,"Need %i grad  ",needPosition);
    lcd_gotoxy(0,1);
    lcd_puts(toLCD);  
}

void main(void)
{
DDRE=(0<<DDE7) | (0<<DDE6) | (0<<DDE5) | (0<<DDE4) | (1<<DDE3) | (1<<DDE2) | (1<<DDE1) | (1<<DDE0);
PORTE=(1<<PORTE7) | (1<<PORTE6) | (0<<PORTE5) | (0<<PORTE4) | (0<<PORTE3) | (0<<PORTE2) | (0<<PORTE1) | (0<<PORTE0);
EIMSK=(1<<INT7) | (1<<INT6) | (0<<INT5) | (0<<INT4) | (0<<INT3) | (0<<INT2) | (0<<INT1) | (0<<INT0);
EIFR=(1<<INTF7) | (1<<INTF6) | (0<<INTF5) | (0<<INTF4) | (0<<INTF3) | (0<<INTF2) | (0<<INTF1) | (0<<INTF0);

lcd_init(16);
#asm("sei")

while (1)
      {
          if((needPosition-nowPosition)>0)
          {
            nowPosition=step((needPosition-nowPosition)/5,nowPosition,direction);      
          }
          else
          {
            nowPosition=step((nowPosition-needPosition)/5,nowPosition,direction); 
          }
      }
}
