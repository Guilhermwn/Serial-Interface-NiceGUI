// Defini��es de bits
#define SBIT_ADON 0 // Bit de controle do ADC
#define SBIT_CHS0 2 // Bits de sele��o do canal ADC
#define SBIT_ADFM 7 // Bit de justifica��o do resultado do ADC

// Conex�es do m�dulo LCD
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

unsigned int num_adc_ldr;  // Vari�vel que recebe o valor do conversor AD para o LDR
unsigned int num_adc_pot;  // Vari�vel que recebe o valor do conversor AD para o potenci�metro
unsigned int num_adc_lm35; // Vari�vel que recebe o valor do conversor AD para o LM35

float V_ldr;     // Valor convertido em volts para o LDR
float V_pot;     // Valor convertido em volts para o potenci�metro
float V_lm35;    // Valor convertido em volts para o LM35

int ldr_int;
int usart_ldr_int;
int pot_int;
int temp_int;
int temp_decimal;
int temp_decimal2;
int Temperatura; // Valor da temperatura

int select = 0; // Contador de sele��o
int intervalo = 100; // Intervalo padr�o de amostragem (0,5 segundos)

void ADC_Iniciar() // Fun��o para inicializar o ADC
{
    ADCON0 = 0x80; // Selecionar clock de convers�o para 20 MHz, Fosc/32 e ADC desabilitado
    ADCON1 = (1 << SBIT_ADFM); // O resultado do ADC � justificado � direita.
}

unsigned int ADC_Ler(unsigned char canal)
{
    ADCON0 = (1 << SBIT_ADON) | (canal << SBIT_CHS0); // Seleciona o canal requerido e habilita o ADC
    Delay_ms(2); // Tempo de aquisi��o (espera que o capacitor de HOLD seja carregado)
    GO_bit = 1; // Inicia a convers�o do ADC
    while (GO_bit == 1); // Aguarda a conclus�o da convers�o, GO_bit ser� zerado automaticamente quando a convers�o estiver completa
    return ((ADRESH << 8) | ADRESL); // Retorna o resultado justificado de 10 bits
}

// Fun��o para mostrar valores com uma casa decimal da Tens�o do LDR
void mostra_ldr(float V)
{
    unsigned int Volt_ldr;
    Volt_ldr = V * 100;
    ldr_int = Volt_ldr / 100;
    Lcd_Chr(2, 1, 48 + ldr_int);
    Lcd_Chr_CP(',');
    ldr_int = (Volt_ldr % 100) / 10;
    Lcd_Chr_CP(48 + ldr_int);
    Lcd_Chr_CP('V');
    Lcd_Chr_CP(' ');
}

// Fun��o para mostrar valores com uma casa decimal da Tens�o do Potenciometro
void mostra_pot(float V)
{
    unsigned int Volt_pot;
    Volt_pot = V * 100;
    pot_int = Volt_pot / 100;
    Lcd_Chr(2, 6, 48 + pot_int);
    Lcd_Chr_CP(',');
    pot_int = (Volt_pot % 100) / 10;
    Lcd_Chr_CP(48 + pot_int);
    Lcd_Chr_CP('V');
    Lcd_Chr_CP(' ');
}

// Fun��o para mostrar valores da convers�o da temperatura do sensor LM35
void mostra_lm35(float V_lm35)
{
    Temperatura = V_lm35 * 10;
    temp_int = Temperatura / 100;
    temp_decimal = (Temperatura % 100) / 10;
    temp_decimal2 = (Temperatura % 10);

    Lcd_Chr(2, 11, 48 + temp_int);
    Lcd_Chr_CP(48 + temp_decimal);
    Lcd_Chr_CP('.');
    Lcd_Chr_CP(48 + temp_decimal2);
    Lcd_Chr_CP(223);          // S�mbolo de grau
    Lcd_Chr_CP('C');
}


/***************** Fun��o para transmitir um caractere *************/
void TXCHR_USART(char dado1)
{
while(TXIF_bit == 0); //espera pelo flag de interrup��o de transmiss�o
TXIF_bit = 0;
TXREG = dado1; //espera que o flag de interrup��o de transmiss�o fique em 1 o
 //qual indica que TXREG est� pronto para outra transmiss�o
}

/******************** Fun��o para transmitir uma string ************/
void TX_STRING_USART(const char *dado)
{
 unsigned int i = 0;
 while (dado[i] != '\0')
 TXCHR_USART(dado[i++]);
}//fim da fun��o TX_STRING_USART()

/**************** Fun��o INCIAR_USART ****************************/
void INICIAR_USART(const unsigned long Baud_Rate)
{
SPBRG = (20000000UL/(long)(16*Baud_Rate))-1; // baud rate para Fosc = 20Mhz e BRGH=1
SYNC_bit = 0; // habilita o modo ass�ncrono
SPEN_bit = 1; //habilita pinos da porta serial
CREN_bit = 1; //habilita a recep��o
TXIE_bit = 0; //desabilita a interrup��o de transmiss�o
RCIE_bit = 0; //desabilita a interrup��o de recep��o
TX9_bit = 0; //transmiss�o de 8 bits de dados
RX9_bit = 0; //recep��o de 8 bits de dados
TXEN_bit = 1; //habilita a transmiss�o
}

void usart_ldr(float v){
    unsigned int usart_Volt_ldr;
    usart_Volt_ldr = V * 100;
    usart_ldr_int = usart_Volt_ldr / 100;
    TXCHR_USART(48 + usart_ldr_int);
    TXCHR_USART(',');
    usart_ldr_int = (usart_Volt_ldr % 100) / 10;
    TXCHR_USART(48 + usart_ldr_int);
    TXCHR_USART('V');
    TXCHR_USART(' ');
}

void interrupt() // Rotina de interrup��o para os intervalos de tempo
{
    if (TMR0IF_bit == 1) {
        TMR0IF_bit = 0; // Limpa a flag de interrup��o do Timer0
        select++;
        if (select >= 50)
        {
            select = 0; // Zera o contador de sele��o
            num_adc_ldr = ADC_Ler(0);  // L� o valor do ADC do canal 0 (LDR)
            num_adc_pot = ADC_Ler(1);  // L� o valor do ADC do canal 1 (Potenci�metro)
            num_adc_lm35 = ADC_Ler(2); // L� o valor do ADC do canal 2 (LM35)

            V_ldr = (num_adc_ldr * 5) / 1024.0;
            V_pot = (num_adc_pot * 5) / 1024.0;
            V_lm35 = num_adc_lm35 * 0.488;
        }
    }
}

void main()
{
    int BOTA = 0;
    int click = 0;
    int click2 = 0;
    int choose = 2;

    // Configura��o dos registradores dos pinos I/O
    ANSEL = 0x07;   // Pino 0 da porta A como anal�gico
    ANSELH = 0;

    OPTION_REG = 0b11010110; // Prescaler de 1:64
    INTCON = 0b10100000;
    TMR0 = 61; // Resulta em 200Hz

    TRISA0_bit = 1; // Pino 0 da porta A como entrada
    TRISA1_bit = 1; // Pino 1 da porta A como entrada
    TRISA2_bit = 1; // Pino 2 da porta A como entrada

    TRISD = 0b11111111; // Pinos da PORT D como entrada
    PORTD = 0x00;

    // Configura��o do LCD
    Lcd_Init();
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Cmd(_LCD_CURSOR_OFF);
    Delay_ms(300);
    // Fim da configura��o do LCD

    ADC_Iniciar(); // Inicializa��o do Conversor AD

    // CONFIGURA��O DO MENU
    while (BOTA == 1)
    {
        Lcd_Out(1, 4, "INTERVALOS");
        Lcd_Out(2, 8, "0");
        Lcd_Chr(2, 9, choose + 48);
        Delay_ms(100);

        // Se RD0 clicado, exibe o proximo intervalo para escolher
        if (PORTD.RD0 == 1) click = 1;
        if (PORTD.RD0 == 0 && click == 1){
            choose++;
            click = 0;
            if (choose >= 6) choose = 1;
        }

        // Se RD1 clicado, confirma o intervalo
        if (PORTD.RD1 == 1) click2 = 1;
        if (PORTD.RD1 == 0 && click2 == 1){
            click2 = 0;
            BOTA = 0;
        }
    }
    // SAIU DA CONFIGURA��O DO MENU

    // Atribui o intervalo selecionado com base na escolha do menu
    if (choose == 1) intervalo = 100; // 0,5 segundos
    if (choose == 2) intervalo = 200; // 1 segundo
    if (choose == 3) intervalo = 2000; // 10 segundos
    if (choose == 4) intervalo = 12000; // 1 minuto
    if (choose == 5) intervalo = 24000; // 720.000 para 1 hora
    
    BRGH_bit = 1; //EUSART funciona a alta velocidade e utiliza a equa��o mostrada para SPBRG.
    INICIAR_USART(19200); //Inicializa com 19200 bps = 19,2 Kbps
    Delay_ms(100); // Espera que o modulo USART se estabilise

    while (1)
    {
        usart_ldr(V_ldr);
        TXCHR_USART(10);
        TXCHR_USART(13);
        mostra_ldr(V_ldr); // Exibe no LCD os valores da tens�o do LDR
        mostra_pot(V_pot); // Exibe no LCD os valores da tens�o do Potenciometro
        mostra_lm35(V_lm35); // Exibe no LCD os valores da temperatura do sensor LM35
        delay_ms(100);
    }
}