// mapeamento do hardware
#define LED     PORTB
#define SINAL   PORTE.B0

// vari�veis globais
const unsigned char mens1[] = "Oi, PC tudo bem?";
const unsigned char mens2[] = "Por favor digite a palavra leds para ligar e desligar os 8 leds a cada 0,5 segundo";
const unsigned char mens3[] = "Agora digite PWM para controlar a intensidade do brilho dos leds";
const unsigned char mens4[] = "Obrigado!";
const unsigned char codi1[] = "leds";
const unsigned char codi2[] = "PWM";
unsigned char digitou, baite;
unsigned char duty, cycle;

// prot�tipo de fun��es
void UART_Init(unsigned char vBRG16, unsigned char vBRGH, unsigned long vSPBRG);
void UART_Chr(unsigned char dado);
void UART_Out(unsigned const char dado[]);
void UART_CrLf();

// tratamento de interrup��es
void interrupt() {
    // tratamento da interrup��o do timer0
    if (TMR0IF_bit) {
        TMR0IF_bit = 0b0;
        if (duty == 0)
            {LED = 0x00; SINAL=0;}
        else if (duty == 100)
            {LED = 0xFF; SINAL=1;}
        else if (PORTB.B0 == 1) {
            LED = 0x00; SINAL=0;
            cycle = 100 - duty;
        } else {
            LED = 0xFF; SINAL=1;
            cycle = duty;
        }
        TMR0 = 256 - cycle;
    }
    
    // Tratamento da interrup��o da recep��o USART
    if (RCIF_bit) {
        RCIF_bit = 0;
        if (FERR_bit || OERR_bit) {
            CREN_bit = 0x00;
            CREN_bit = 0x01;
        }
        baite = RCREG;
        digitou = 1;
    }
}

// programa principal
void main() {
    // defina e inicialize vari�veis
    unsigned short tamanho, posicao;
    duty = 0;
    baite = 0;
    digitou = 0;

    // configure registradores gerais
    ANSEL = 0x00;
    ANSELH = 0x00;
    C1ON_bit = 0b0;
    C2ON_bit = 0b0;
    INTCON.PEIE = 1;
    INTCON.GIE = 1;
    
    // configure portas de entrada e sa�da
    TRISB      = 0x00;
    TRISE0_bit = 0b0;

    // configura timer0 e sua interrup��o
    OPTION_REG = 0x05;
    T0IF_bit = 0b0;
    T0IE_bit = 0b1;
    cycle = duty;
    TMR0 = 256 - cycle;
    
    // inicialize hardware
    LED = 0x00; SINAL=0;
    UART_Init(0, 1, 21);
    
    // fase de verificacao
    UART_CrLf();
    UART_Out(mens1);
    UART_CrLf();
    while (baite!=13){
        if(digitou == 1){
            digitou = 0;
            UART_Chr(baite);
        }
    }

    UART_CrLf();
    UART_Out(mens2);
    UART_CrLf();
    tamanho = sizeof(codi1) - 1;
    posicao = 0;
    while (posicao < tamanho) {
        if (digitou == 1) {
            digitou = 0;
            UART_Chr(baite);
            if (baite == codi1[posicao]) {
                posicao++;
            } else {
                UART_CrLf();
                posicao = 0;
            }
        }
    }
    while(1){
        LED=0x00;
        Delay_ms(500);
        LED=0xFF;
        Delay_ms(500);
        if(digitou==1){
            digitou=0;
            break;
        }
    }
    digitou=0;
    
    UART_CrLf();
    UART_Out(mens3);
    UART_CrLf();
    tamanho = sizeof(codi2) - 1;
    posicao = 0;
    while (posicao < tamanho) {
        if (digitou == 1) {
            digitou = 0;
            UART_Chr(baite);
            if (baite == codi2[posicao]) {
                posicao++;
            } else {
                UART_CrLf();
                posicao = 0;
            }
        }
    }
    // loop infinito
    UART_CrLf();
    UART_Out("PWM liberado");
    UART_CrLf();
    while (1) {
        duty = duty+10;
        Delay_ms(250);
        if(duty==100) duty=0;
    }
} // fim main

// Inicialize a comunica��o serial
void UART_Init(unsigned char vBRG16, unsigned char vBRGH, unsigned long vSPBRG) {
    // Configure Baud rate
    BRGH_bit = vBRGH;
    BRG16_bit = vBRG16;
    SPBRGH = vSPBRG / 256;
    SPBRG = vSPBRG % 256;
    SYNC_bit = 0b0;
    SPEN_bit = 0b1;
    TX9_bit = 0b0;
    TXEN_bit = 0b1;
    CREN_bit = 0b1;
    TXIE_bit = 0b0;
    RCIE_bit = 0b1;
}

// Envia um caracter para a porta serial
void UART_Chr(unsigned char dado) {
    while (TRMT_bit == 0);
    TXREG = dado;
}

// Envia uma string para a porta serial sem o CR+LF
void UART_Out(unsigned const char dado[]) {
    int i = 0;
    while (dado[i] != '\0')
        UART_Chr(dado[i++]);
}

// Envia Cr+LF para a porta serial
void UART_CrLf() {
    UART_Chr(13);
    UART_Chr(10);
}