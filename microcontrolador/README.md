# Código para Microcontrolador PIC16F887: Comunicação Serial com Interface Web

Este código foi desenvolvido para ser compilado utilizando o **MikroC** e executado em um microcontrolador PIC16F887, que recebe dados eFnviados via comunicação serial pela interface web construída com **NiceGUI**. A interface envia os comandos `UP` e `DOWN`, que controlam o estado dos pinos da porta B do microcontrolador. O código utiliza a biblioteca **USART.h** incluída nos arquivos do repositório para gerenciar a comunicação serial.

## Descrição

O microcontrolador recebe os comandos enviados pela interface serial e ajusta o valor de `PORTB` com base nas seguintes regras:

- **Comando 'U' (UP)**: O valor de `PORTB` é deslocado à esquerda (<<), acendendo LEDs conectados aos pinos da porta B de forma sequencial. Se o valor atingir zero, ele é reiniciado como `1`.
- **Comando 'D' (DOWN)**: O valor de `PORTB` é deslocado à direita (>>), apagando LEDs de forma inversa. Se o valor atingir zero, ele é reiniciado como `128`.

## Funcionalidade do Código

1. **Configuração Inicial**: Os pinos analógicos são desativados e a porta B é configurada como saída. A comunicação serial é inicializada a 9600 bps.
   
2. **Recepção de Dados**: O microcontrolador entra em um loop infinito esperando por dados recebidos via serial. Quando um comando é recebido, ele é interpretado e processado.

3. **Ação com Base no Comando**:
   - Se o comando for **'U'**, o valor de `PORTB` é deslocado à esquerda, acendendo o próximo LED.
   - Se o comando for **'D'**, o valor de `PORTB` é deslocado à direita, apagando o LED atual.

## Fluxo de Execução

1. **Inicialização**:
   - Configura os pinos analógicos como digitais.
   - Define a porta B como saída.
   - Inicializa a comunicação serial em 9600 bps.
   
2. **Loop Principal**:
   - O código espera até que um dado seja recebido via comunicação serial.
   - Quando o dado é recebido, ele é processado e uma ação correspondente é executada.
   - O valor de `PORTB` é atualizado de acordo com o comando recebido (`'U'` ou `'D'`), controlando os LEDs conectados à porta.

## Exemplo de Execução

- **Comando 'U'**: O LED na próxima posição (à esquerda) será aceso. Se todos os LEDs estiverem apagados, o primeiro LED será aceso.
- **Comando 'D'**: O LED na posição anterior (à direita) será apagado. Se todos os LEDs estiverem apagados, o último LED será aceso.

## Requisitos

- **Microcontrolador**: O código deve ser adaptado para o microcontrolador que você está utilizando. No exemplo, utiliza-se um microcontrolador que suporta as funções definidas no arquivo **USART.h**.
- **USART.h**: Biblioteca necessária para gerenciar a comunicação serial.
- **Compilador MikroC**: O código foi desenvolvido para ser compilado com o compilador **MikroC**.
- **Interface de Comunicação Serial**: A interface envia os comandos via comunicação serial para o microcontrolador.

## Instalação

1. Copie o código para o seu ambiente de desenvolvimento **MikroC**.
2. Verifique se o arquivo **USART.h** está disponível e configurada corretamente no seu ambiente.
3. Compile e envie o código para o microcontrolador.
4. Conecte o microcontrolador ao computador via interface serial e execute a interface web para enviar comandos.

## Estrutura do Código

```c
#include "USART.h"

void main() {
    char recebe;
    unsigned char j = 0;

    // Configurar pinos AN como digital
    ANSEL = ANSELH = 0;
    C1ON_bit = C2ON_bit = 0; // Desabilita os comparadores
    TRISB = 0;               // Porta B saída
    PORTB = j;

    INICIAR_USART(9600); // Inicializa com 9600 bps
    Delay_ms(100);

    while (1) {
        // Laço infinito
        if (Dado_Pronto_USART()) {
            // Se dado foi recebido,
            recebe = RXCHR_USART();

            if (recebe == 'U') {
                j <<= 1;
                if (j == 0)
                    j = 1;
            }

            if (recebe == 'D') {
                j >>= 1;
                if (j == 0)
                    j = 128;
            }

            PORTB = j;
        } // fim do if
    } // fim do while
} // fim do main
```

## Como Contribuir

Contribuições são bem-vindas! Se você tiver sugestões de melhorias ou encontrar algum problema, sinta-se à vontade para abrir um **pull request** ou **issue**.
