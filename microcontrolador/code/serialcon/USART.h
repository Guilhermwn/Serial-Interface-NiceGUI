/**
 * @brief Inicializa o módulo USART do microcontrolador PIC16F887.
 * 
 * Esta função configura os registradores necessários para iniciar o módulo USART
 * em modo assíncrono, habilitando a transmissão e recepção de dados seriais.
 * 
 * @param Baud_Rate A taxa de baud desejada para a comunicação serial.
 *                  A fórmula usada para calcular o valor do registrador SPBRG é:
 *                  SPBRG = (Fosc / (64 * Baud_Rate)) - 1,
 *                  onde Fosc é a frequência do oscilador, que neste caso é 20 MHz.
 * 
 * @note Esta função assume que a frequência do oscilador (Fosc) é de 20 MHz.
 */
void INICIAR_USART(const unsigned long Baud_Rate)
{
    SPBRG = (20000000UL / (long)(64UL * Baud_Rate)) - 1; // baud rate para Fosc = 20Mhz
    SYNC_bit = 0;                                        // habilita o modo assíncrono
    SPEN_bit = 1;                                        // habilita pinos da porta serial
    CREN_bit = 1;                                        // habilita a recepção
    TXIE_bit = 0;                                        // desabilita a interrupção de transmissão
    RCIE_bit = 0;                                        // desabilita a interrupção de recepção
    TX9_bit = 0;                                         // transmissão de 8 bits de dados
    RX9_bit = 0;                                         // recepção de 8 bits de dados
    TXEN_bit = 1;                                        // habilita a transmissão
}

/**
 * @brief Transmite um caractere via módulo USART do microcontrolador PIC16F887.
 * 
 * Esta função envia um caractere via USART, aguardando até que o registrador de 
 * transmissão esteja pronto para aceitar um novo dado.
 * 
 * @param dado O caractere a ser transmitido.
 */
void TXCHR_USART(char dado)
{
    while (TXIF_bit == 0)
        ;         // espera pelo flag de interrupção de transmissão
    TXREG = dado; // espera que o flag de interrupção de transmissão fique em 1 o
    // qual indica que TXREG está pronto para outra transmissão
}

/**
 * @brief Recebe um caractere via módulo USART do microcontrolador PIC16F887.
 * 
 * Esta função aguarda até que um caractere seja recebido via USART e então o retorna.
 * Se ocorrer um erro de overflow durante a recepção, o bit de erro é limpo antes de
 * continuar a operação.
 * 
 * @return O caractere recebido.
 */
unsigned char RXCHR_USART()
{
    unsigned char dado;
    while (RCIF_bit == 0)
        ; // espera pelo flag de interrupção de recepção
    RCIF_bit = 0;
    dado = RCREG;
    if (OERR_bit) // Se acontecer algum erro de overflow
    {
        CREN_bit = 0;
        // para limpar o bit de overflow
        CREN_bit = 1;
    }
    return dado; /*recebido no registrador RCREG e retorna para o programa principal*/
} // fim de função de recepção de caracteres

/**
 * @brief Verifica se um dado foi recebido via módulo USART do microcontrolador PIC16F887.
 * 
 * Esta função retorna o estado do bit de flag de interrupção de recepção (RCIF),
 * indicando se um dado foi recebido e está disponível no registrador de recepção (RCREG).
 * 
 * @return O valor do bit RCIF (1 se um dado foi recebido, 0 caso contrário).
 */
char Dado_Pronto_USART()
{
    return RCIF_bit; // retorna o bit RCIF (registrador PIR1, bit 5)
} // fim da função Dado_Pronto_USART


/**
 * @brief Transmite uma string via módulo USART do microcontrolador PIC16F887.
 * 
 * Esta função envia uma sequência de caracteres (string) via USART, caractere por caractere,
 * até encontrar o caractere nulo ('\0'), que indica o final da string.
 * 
 * @param dado O ponteiro para a string a ser transmitida.
 */
void TX_STRING_USART(const char *dado)
{
    unsigned int i = 0;
    while (dado[i] != '\0')
        TXCHR_USART(dado[i++]);
} // fim da função TX_STRING_USART()
