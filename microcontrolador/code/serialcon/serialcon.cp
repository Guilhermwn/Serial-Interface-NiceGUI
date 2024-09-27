#line 1 "C:/Guilhermwn/DynamicProjects/YouTube-tutorials/nicegui-pic/microcontrolador/code/serialcon/serialcon.c"
#line 1 "c:/guilhermwn/dynamicprojects/youtube-tutorials/nicegui-pic/microcontrolador/code/serialcon/usart.h"

void INICIAR_USART(const unsigned long Baud_Rate)
{
 SPBRG = (20000000UL / (long)(64UL * Baud_Rate)) - 1;
 SYNC_bit = 0;
 SPEN_bit = 1;
 CREN_bit = 1;
 TXIE_bit = 0;
 RCIE_bit = 0;
 TX9_bit = 0;
 RX9_bit = 0;
 TXEN_bit = 1;
}


void TXCHR_USART(char dado)
{
 while (TXIF_bit == 0)
 ;
 TXREG = dado;

}


unsigned char RXCHR_USART()
{
 unsigned char dado;
 while (RCIF_bit == 0)
 ;
 RCIF_bit = 0;
 dado = RCREG;
 if (OERR_bit)
 {
 CREN_bit = 0;

 CREN_bit = 1;
 }
 return dado;
}


char Dado_Pronto_USART()
{
 return RCIF_bit;
}



void TX_STRING_USART(const char *dado)
{
 unsigned int i = 0;
 while (dado[i] != '\0')
 TXCHR_USART(dado[i++]);
}
#line 3 "C:/Guilhermwn/DynamicProjects/YouTube-tutorials/nicegui-pic/microcontrolador/code/serialcon/serialcon.c"
void main()
{
 char recebe;
 unsigned char j = 0;


 ANSEL = ANSELH = 0;
 C1ON_bit = C2ON_bit = 0;
 TRISB = 0;
 PORTB = j;

 INICIAR_USART(9600);
 Delay_ms(100);

 while (1)
 {

 if (Dado_Pronto_USART())
 {

 recebe = RXCHR_USART();

 if (recebe == 'U')
 {
 j <<= 1;
 if (j == 0)
 j = 1;
 }

 if (recebe == 'D')
 {
 j >>= 1;
 if (j == 0)
 j = 128;
 }

 PORTB = j;
 }
 }
}
