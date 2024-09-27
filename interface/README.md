# Projeto de Interface Web para Comunicação Serial com Microcontrolador

Este projeto cria uma interface web utilizando a biblioteca **NiceGUI**, que renderiza a interface usando o **Quasar Framework** baseado em Vue.js. A interface permite ao usuário se comunicar com um microcontrolador via portas seriais, utilizando a biblioteca **pyserial** para o gerenciamento das conexões. 

## Funcionalidades

- **Interface Web**: Interface gráfica construída com **NiceGUI** e renderizada com **Quasar Framework**, executada diretamente no navegador ou em uma janela local do computador.
- **Conexão Serial**: Detecção automática das portas seriais disponíveis e comunicação com microcontroladores através da biblioteca **pyserial**.
- **Registro de Log**: A interface registra as interações (comandos "UP" e "DOWN") e exibe o histórico na tela.
- **Atualização Dinâmica**: A lista de portas seriais é atualizada em tempo real com a opção de busca.

## Tecnologias Utilizadas

- **NiceGUI**: Para construção de interfaces gráficas renderizadas em **Quasar VUE JS**.
- **Pyserial**: Biblioteca usada para comunicação serial com dispositivos conectados ao computador.
- **Python**: Linguagem de programação principal do projeto.
- **Quasar VUE JS**: Framework frontend utilizado para renderização da interface.

## Instalação

1. Clone este repositório:
   ```bash
   git clone https://github.com/usuario/repo.git
   cd repo
   ```

2. Crie e ative um ambiente virtual:
   ```bash
   python -m venv venv
   venv/bin/activate  # Linux/MacOS
   venv\Scripts\activate  # Windows
   ```

3. Instale as dependências necessárias:
   ```bash
   pip install nicegui pyserial
   ```

## Como Executar

1. Conecte seu microcontrolador ao computador via uma porta serial.

2. Execute o script:
   ```bash
   python main.py
   ```

3. A interface estará disponível no seu navegador em `http://localhost:4000`.

4. Selecione a porta serial correspondente ao seu dispositivo na interface.

5. Clique nos botões **UP** ou **DOWN** para enviar comandos para o microcontrolador via serial. O log das operações será exibido em tempo real na interface.


## Personalização

- **Endereço IP**: Por padrão, a aplicação roda em `localhost`. No entanto, é possível definir um endereço IP customizado ao iniciar o servidor modificando a linha:
  ```python
  ui.run(host='192.168.1.10', port=4000)
  ```

- **Estilo da Interface**: A interface suporta modo escuro, configurado com a opção `dark=True`. Você pode remover ou alterar esse comportamento conforme sua preferência.

## Requisitos

- **Python 3.8+**
- **Biblioteca NiceGUI**
- **Biblioteca pyserial**

## Contribuições

Contribuições são bem-vindas! Sinta-se à vontade para abrir um **pull request** ou **issue** se encontrar algum problema ou tiver sugestões de melhorias.
