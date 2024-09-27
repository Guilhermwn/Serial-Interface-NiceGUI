import glob
import sys
import serial

# DETECTOR DE PORTAS CONECTADAS
def serial_ports():
    """ Lista o nome das portas atualmente conectadas

    :raises EnvironmentError:
        Sistema operacional ou plataforma não suportada
    :returns:
        Uma lista com as portas seriais atualmente conectadas
    """
    if sys.platform.startswith('win'):  # Verifica se o sistema operacional é Windows
        ports = ['COM%s' % (i + 1) for i in range(256)]  # Lista de portas COM no Windows
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):  # Verifica se o sistema operacional é Linux ou Cygwin
        # Lista todas as portas seriais no sistema Linux ou Cygwin, excluindo o terminal atual
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):  # Verifica se o sistema operacional é macOS
        # Lista todas as portas seriais no sistema macOS
        ports = glob.glob('/dev/tty.*')
    else:  # Se o sistema operacional não for suportado, levanta um erro
        raise EnvironmentError('Unsupported platform')

    result = []  # Inicializa uma lista para armazenar as portas seriais disponíveis
    for port in ports:  # Itera sobre todas as portas seriais encontradas
        try:  # Tenta abrir a porta serial
            s = serial.Serial(port)
            s.close()  # Fecha a porta serial
            result.append(port)  # Adiciona a porta à lista de portas disponíveis se abrir e fechar com sucesso
        except (OSError, serial.SerialException):  # Captura exceções que podem ocorrer ao abrir a porta serial
            pass  # Não faz nada se ocorrer uma exceção e continua para a próxima porta
    # if result == []:
    #     return False
    # else:
    return result  # Retorna a lista de portas seriais disponíveis