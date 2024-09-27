/*
COMANDOS:
Tudo bem
Ta bom
LEDS
sem problema
PWM
PARAR
*/

// Definição das condições do loop
#define ever ;;

// Com este valor o Buffer deve armazena 15 caracteres, enter('\r') e o nulo('\0')
#define MAX_BUFFER 15 

// Início das configurações do LCD
sbit LCD_RS at RC5_bit;
sbit LCD_EN at RC4_bit;
sbit LCD_D4 at RC3_bit;
sbit LCD_D5 at RC2_bit;
sbit LCD_D6 at RC1_bit;
sbit LCD_D7 at RC0_bit;

sbit LCD_RS_Direction at TRISC5_bit;
sbit LCD_EN_Direction at TRISC4_bit;
sbit LCD_D4_Direction at TRISC3_bit;
sbit LCD_D5_Direction at TRISC2_bit;
sbit LCD_D6_Direction at TRISC1_bit;
sbit LCD_D7_Direction at TRISC0_bit;
// Fim das configurações do LCD

char BufferRX[MAX_BUFFER + 1]; //quando a string se quer mostrar no LCD (16 caracteres) esta deve ter 14 caracteres, enter e o nulo.
unsigned char dado;
unsigned char flag;
unsigned int indiceBuf = 0;
int sent = 0;
int leds = 0;
int buzzer = 0;

unsigned char duty = 0;
unsigned int cycle = 0;
unsigned int valor = 0;

// Função para iniciar a comunicação serial
void INICIAR_USART(int Baud_Rate){
    unsigned int N;
    N = (20000000UL / (long)(4UL * Baud_Rate)) - 1; // baud rate para Fosc = 20Mhz
    SPBRG = N & 0XFF;
    SPBRGH = N >> 8;
    SYNC_bit = 0; // habilita o modo assincrono
    SPEN_bit = 1; // habilita pinos da porta serial
    CREN_bit = 1; // habilita a recepção
    TXIE_bit = 0; // desabilita a interrupção de trasmissão
    RCIE_bit = 0; // desabilita a interrupção de recepção
    TX9_bit = 0; // trasmissão de 8 bits de dados
    RX9_bit = 0; // recepção de 8 bits de dados
    TXEN_bit = 1; // habilita a trasmissão
} // fim da função de inicio da comunicação

// Função para transmitir um caractere 
void TXCHR_USART(char dado){
    while (TXIF_bit == 0); // espera pelo flag de interrupção de trasmissão
    TXREG = dado; // espera que o flag de interrupção de trasmissão fique em 1 o qual indica que TXREG está pronto para outra trasmissão
} // fim da transmissão de caractere

// Função para transmitir uma string
void TX_STRING_USART(const char *linha){
    unsigned int i = 0;
    while (linha[i] != '\0')
    TXCHR_USART(linha[i++]);
} // fim da transmissção de string

// --- Rotina de Interrupção ---
void interrupt(){
    //Houve interrupção na USART?
    if(RCIF_bit) 
    { //Sim...
        RCIF_bit = 0; //Limpa flag
        if(FERR_bit || OERR_bit) //Ocorreu erro de framing ou overrun?
        { //Sim...
            CREN_bit = 0x00; //Limpa status de recebimento
            CREN_bit = 0x01;
        } //fim de teste de erro
        TXREG = RCREG; //transmitimos o que recebemos
        dado = RCREG; //a variavel recebe o valor do dado recebido
        BufferRX[indiceBuf] = dado; //armazenando o dado no vetor
        if(BufferRX[indiceBuf] == 0x0A) //caractere ENTER
        {
            BufferRX[indiceBuf] = '\0';
            indiceBuf = 0;
            flag = 1;
        } //fim do if (Buffer...)
        else
        {
        indiceBuf++;
        }//fim do else

        if(indiceBuf >= MAX_BUFFER)
        {
        indiceBuf = 0;
        } //fim do if
    } // fim da interrupção do USART
}
// --- Fim da Rotina de Interrupção ---


void main(){
    BRGH_bit = 1; //EUSART funciona a alta velocidade e utiliza a equação mostrada para SPBRG.
    BRG16_bit = 1;
    INICIAR_USART(19200); // Inicializa com 19200 bps
    Delay_ms(100);
    /******* Configuração da interrupção de recepção USART *******/
    RCIF_bit = 0; //desabilitamos o flag de interrupção de recepção USART
    RCIE_bit = 1; //Habilitamos a interrupção de recepção USART
    GIE_bit = 1; //Habilitamos a interrupção global
    PEIE_bit = 1; //Habilitamos a interrupção de perifericos
    // Fim das configurações de interrupção de recepção USART

    OPTION_REG = 0xC4; // Prescaler de 1:32

    TRISB = 0; // Habilitamos PORTB como saida
    PORTB = 0; // PORTB em nivel baixo
    TRISD = 0; // Habilitamos PORTD como saida
    PORTD = 0; // PORTD em nivel baixo
    TRISE = 0; // Habilitamos PORTD como saida
    PORTE = 0; // PORTD em nivel baixo
    TRISC = 0;
    PORTC = 0;

    Lcd_Init();
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Cmd(_LCD_CURSOR_OFF);
    //Delay_ms(300);
    

    TX_STRING_USART("Oi, PC tudo bem?");
    TXCHR_USART(10); TXCHR_USART(10);
        Lcd_Out(1, 2, "LABORATORIO  4");
        Lcd_Out(2, 2, "EXPERIMENTO  1");
    for(ever){
        
        
        if (flag == 1){
            if (sent == 0 && BufferRX[0] == 'T' && BufferRX[1] == 'u' && BufferRX[2] == 'd' && BufferRX[3] == 'o'  && BufferRX[4] == ' ' && BufferRX[5] == 'b' && BufferRX[6] == 'e' && BufferRX[7] == 'm'){
                Lcd_Cmd(_LCD_CLEAR);
                TXCHR_USART(10);
                TX_STRING_USART("Por favor digita a palavra 'LEDS' para ligar e desligar os 8 leds a cada 0,5 segundos.");
                TXCHR_USART(10); TXCHR_USART(10);
                Lcd_Out(1, 1, "Oi, Pc tudo bem?");
                Lcd_Out(2, 1, BufferRX);
                flag = 0;
                sent = 1;
            }
            if (sent == 1 && BufferRX[0] == 'L' && BufferRX[1] == 'E' && BufferRX[2] == 'D' && BufferRX[3] == 'S'){
                Lcd_Cmd(_LCD_CLEAR);
                TXCHR_USART(10);
                TX_STRING_USART("Agora digita a palavra 'PWM' para controlar a intensidade do brilho dos leds.");
                TXCHR_USART(10); TXCHR_USART(10);
                Lcd_Out(1, 1, "Digite 'LEDS'");
                Lcd_Out(2, 1, BufferRX);
                leds = 1;
                flag = 0;
                sent = 2;
            }

            if (sent == 2 && BufferRX[0] == 'P' && BufferRX[1] == 'W' && BufferRX[2] == 'M'){
                Lcd_Cmd(_LCD_CLEAR);
                leds = 2;
                TXCHR_USART(10);
                TX_STRING_USART("Obrigado.");
                TXCHR_USART(10); TXCHR_USART(10);
                TX_STRING_USART("Por ultimo digita a palavra 'PARAR' para que eu possa parar a acao anterior e que o buzzer apite por 3 segundos.");
                TXCHR_USART(10); TXCHR_USART(10);
                TX_STRING_USART("Assim nos poderemos comecar de novo.");
                TXCHR_USART(10); TXCHR_USART(10);
                Lcd_Out(1, 1, "Digite 'PWM'");
                Lcd_Out(2, 1, BufferRX);
                
                flag = 0;
                sent = 3;
            }
            
            if(sent == 3 && BufferRX[0] == 'P' && BufferRX[1] == 'A' && BufferRX[2] == 'R' && BufferRX[3] == 'A' && BufferRX[4] == 'R'){
                Lcd_Cmd(_LCD_CLEAR);
                TXCHR_USART(10);
                TX_STRING_USART("...Programa Finalizado...");
                Lcd_Out(1, 1, "Digite 'PARAR'");
                Lcd_Out(2, 1, BufferRX);
                PORTB = 0;
                leds = 0;
                buzzer = 1;
                flag = 0;
                sent++;
            }
            delay_ms(300);
        } // fim comparação de Flag
        
        while (leds == 1){
                PORTB = 0XFF;
                delay_ms(500);
                PORTB = 0X00;
                delay_ms(500);
                if(sent != 1) break;
        }

        while(leds == 2){
            if(duty == 0){
                Lcd_Out(2, 6, "0"); Lcd_Chr(2,7,37); Lcd_Out(2,8,"  ");
                PORTB = 0;
                delay_us(2000);
            }
            if(duty == 1){
                Lcd_Out(2, 6, "10"); Lcd_Chr(2,8,37);
                PORTB = 255;
                delay_us(100);
                PORTB = 0;
                delay_us(900);
            }
            if(duty == 2){
                Lcd_Out(2, 6, "20"); Lcd_Chr(2,8,37);
                PORTB = 255;
                delay_us(200);
                PORTB = 0;
                delay_us(800);
            }
            if(duty == 3){
                Lcd_Out(2, 6, "30"); Lcd_Chr(2,8,37);
                PORTB = 255;
                delay_us(300);
                PORTB = 0;
                delay_us(700);
            }
            if(duty == 4){
                Lcd_Out(2, 6, "40"); Lcd_Chr(2,8,37);
                PORTB = 255;
                delay_us(400);
                PORTB = 0;
                delay_us(600);
            }
            if(duty == 5){
                Lcd_Out(2, 6, "50"); Lcd_Chr(2,8,37);
                PORTB = 255;
                delay_us(500);
                PORTB = 0;
                delay_us(500);
            }
            if(duty == 6){
                Lcd_Out(2, 6, "60"); Lcd_Chr(2,8,37);
                PORTB = 255;
                delay_us(600);
                PORTB = 0;
                delay_us(400);
            }
            if(duty == 7){
                Lcd_Out(2, 6, "70"); Lcd_Chr(2,8,37);
                PORTB = 255;
                delay_us(700);
                PORTB = 0;
                delay_us(300);
            }
            if(duty == 8){
                Lcd_Out(2, 6, "80"); Lcd_Chr(2,8,37);
                PORTB = 255;
                delay_us(800);
                PORTB = 0;
                delay_us(200);
            }
            if(duty == 9){
                Lcd_Out(2, 6, "90"); Lcd_Chr(2,8,37);
                PORTB = 255;
                delay_us(900);
                PORTB = 0;
                delay_us(100);
            }
            if(duty == 10){
                Lcd_Out(2, 6, "100"); Lcd_Chr(2,9,37);
                PORTB = 255;
                delay_us(2000);
            }
            cycle++;
            if(cycle == 200){
                duty++;
                cycle = 0;
            }
            if(duty > 10){
                duty=0;
            }
            if(sent !=2) break;
        }

        if(buzzer == 1){
            PORTD.B7 = 1;
            delay_ms(2000);
            PORTD.B7 = 0;
            buzzer = 0;
        }
    } // fim do forever loop
} // fim do main