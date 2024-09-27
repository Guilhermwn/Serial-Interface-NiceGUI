# Projeto: Interface Web para Comunicação Serial com Microcontrolador

Este repositório contém o código de um projeto que combina uma **interface web** interativa, desenvolvida em **Python** usando a biblioteca **NiceGUI**, com um **microcontrolador** que se comunica via **serial**. A interface permite enviar comandos ao microcontrolador, que responde ajustando saídas digitais (como LEDs conectados à porta B). O projeto foi criado para facilitar o controle de hardware a partir de uma interface web amigável.

## Visão Geral do Projeto

O projeto é dividido em duas partes principais:

### 1. **Interface Web em Python**
A interface é desenvolvida utilizando **NiceGUI**, uma biblioteca que facilita a criação de interfaces web modernas renderizadas com **Quasar Framework** baseado em Vue.js. A interface permite ao usuário:

- **Selecionar portas seriais** disponíveis no computador.
- **Enviar comandos** de controle (como `UP` e `DOWN`) para o microcontrolador via comunicação serial.
- **Exibir um log** em tempo real das ações executadas.

A comunicação com o microcontrolador é feita através da biblioteca **serial** em Python para listar as portas seriais conectadas ao sistema e realizar a comunicação.

#### Principais Funcionalidades da Interface:
- **Atualização de Portas Seriais**: Possibilidade de atualizar a lista de portas seriais conectadas ao computador.
- **Envio de Comandos**: Botões interativos para enviar os comandos `UP` e `DOWN` via serial.
- **Registro de Logs**: Mostra em tempo real os comandos enviados e a hora em que foram executados.

### 2. **Código do Microcontrolador**
O código do microcontrolador, desenvolvido em **C** e compilado com **MikroC**, recebe os comandos enviados via serial e realiza o controle da porta B. Dependendo do comando recebido:

- **Comando 'U' (UP)**: Desloca o valor da porta B para a esquerda, acendendo LEDs sequencialmente.
- **Comando 'D' (DOWN)**: Desloca o valor da porta B para a direita, apagando os LEDs de forma inversa.

#### Principais Funcionalidades do Microcontrolador:
- **Recepção de Dados via Serial**: Utiliza a biblioteca **USART** para receber dados e controlar as saídas da porta B.
- **Controle de LEDs**: O valor de `PORTB` é ajustado conforme os comandos `UP` ou `DOWN`, controlando LEDs conectados à porta.

## Estrutura do Repositório

- **/interface/**: Contém o código da interface web em Python.
- **/microcontrolador/**: Contém o código em C para o microcontrolador, incluindo a configuração da comunicação serial.
- **README.md**: Documentação geral do projeto (este arquivo).

## Requisitos

### Para a Interface em Python:
- **Python 3.x**
- **NiceGUI**: Para a criação da interface web.
- **PySerial**: Para a comunicação serial com o microcontrolador.

### Para o Código do Microcontrolador:
- **Compilador MikroC**: O código do microcontrolador foi desenvolvido para ser compilado com o **MikroC**.
- **Biblioteca USART.h**: Biblioteca necessária para a comunicação serial no microcontrolador.

## Como Executar o Projeto

### 1. Interface Web em Python
1. Instale as dependências:
   ```bash
   pip install nicegui pyserial
   ```
2. Execute o código da interface:
   ```bash
   python main.py
   ```

### 2. Microcontrolador
1. Copie o código na pasta `/microcontrolador/` para o seu ambiente de desenvolvimento **MikroC**.
2. Compile e envie o código para o microcontrolador.
3. Conecte o microcontrolador ao seu computador via interface serial.

## Contribuições

Se você deseja contribuir com o projeto, fique à vontade para abrir um **pull request** ou relatar problemas na aba **Issues**. Toda ajuda é bem-vinda!
