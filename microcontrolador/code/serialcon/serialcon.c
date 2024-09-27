#include "USART.h"

void main()
{
    char recebe;
    unsigned char j = 0;

    // Configurar pinos AN como digital
    ANSEL = ANSELH = 0;
    C1ON_bit = C2ON_bit = 0; // Desabilita os comparadores
    TRISB = 0;               // Porta B saída
    PORTB = j;

    INICIAR_USART(9600); // Inicializa com 9600 bps
    Delay_ms(100);

    while (1)
    {
        // Laço infinito
        if (Dado_Pronto_USART())
        {
            // Se dado foi recebido,
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
        } // fim do if
    } // fim do while
} // fim do main