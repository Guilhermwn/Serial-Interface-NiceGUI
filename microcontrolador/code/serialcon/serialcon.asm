
_INICIAR_USART:

;usart.h,2 :: 		void INICIAR_USART(const unsigned long Baud_Rate)
;usart.h,4 :: 		SPBRG = (20000000UL / (long)(64UL * Baud_Rate)) - 1; // baud rate para Fosc = 20Mhz
	MOVLW      6
	MOVWF      R0+0
	MOVF       FARG_INICIAR_USART_Baud_Rate+0, 0
	MOVWF      R4+0
	MOVF       FARG_INICIAR_USART_Baud_Rate+1, 0
	MOVWF      R4+1
	MOVF       FARG_INICIAR_USART_Baud_Rate+2, 0
	MOVWF      R4+2
	MOVF       FARG_INICIAR_USART_Baud_Rate+3, 0
	MOVWF      R4+3
	MOVF       R0+0, 0
L__INICIAR_USART16:
	BTFSC      STATUS+0, 2
	GOTO       L__INICIAR_USART17
	RLF        R4+0, 1
	RLF        R4+1, 1
	RLF        R4+2, 1
	RLF        R4+3, 1
	BCF        R4+0, 0
	ADDLW      255
	GOTO       L__INICIAR_USART16
L__INICIAR_USART17:
	MOVLW      0
	MOVWF      R0+0
	MOVLW      45
	MOVWF      R0+1
	MOVLW      49
	MOVWF      R0+2
	MOVLW      1
	MOVWF      R0+3
	CALL       _Div_32x32_U+0
	DECF       R0+0, 0
	MOVWF      SPBRG+0
;usart.h,5 :: 		SYNC_bit = 0;                                        // habilita o modo assíncrono
	BCF        SYNC_bit+0, BitPos(SYNC_bit+0)
;usart.h,6 :: 		SPEN_bit = 1;                                        // habilita pinos da porta serial
	BSF        SPEN_bit+0, BitPos(SPEN_bit+0)
;usart.h,7 :: 		CREN_bit = 1;                                        // habilita a recepção
	BSF        CREN_bit+0, BitPos(CREN_bit+0)
;usart.h,8 :: 		TXIE_bit = 0;                                        // desabilita a interrupção de transmissão
	BCF        TXIE_bit+0, BitPos(TXIE_bit+0)
;usart.h,9 :: 		RCIE_bit = 0;                                        // desabilita a interrupção de recepção
	BCF        RCIE_bit+0, BitPos(RCIE_bit+0)
;usart.h,10 :: 		TX9_bit = 0;                                         // transmissão de 8 bits de dados
	BCF        TX9_bit+0, BitPos(TX9_bit+0)
;usart.h,11 :: 		RX9_bit = 0;                                         // recepção de 8 bits de dados
	BCF        RX9_bit+0, BitPos(RX9_bit+0)
;usart.h,12 :: 		TXEN_bit = 1;                                        // habilita a transmissão
	BSF        TXEN_bit+0, BitPos(TXEN_bit+0)
;usart.h,13 :: 		}
L_end_INICIAR_USART:
	RETURN
; end of _INICIAR_USART

_TXCHR_USART:

;usart.h,16 :: 		void TXCHR_USART(char dado)
;usart.h,18 :: 		while (TXIF_bit == 0)
L_TXCHR_USART0:
	BTFSC      TXIF_bit+0, BitPos(TXIF_bit+0)
	GOTO       L_TXCHR_USART1
;usart.h,19 :: 		;         // espera pelo flag de interrupção de transmissão
	GOTO       L_TXCHR_USART0
L_TXCHR_USART1:
;usart.h,20 :: 		TXREG = dado; // espera que o flag de interrupção de transmissão fique em 1 o
	MOVF       FARG_TXCHR_USART_dado+0, 0
	MOVWF      TXREG+0
;usart.h,22 :: 		}
L_end_TXCHR_USART:
	RETURN
; end of _TXCHR_USART

_RXCHR_USART:

;usart.h,25 :: 		unsigned char RXCHR_USART()
;usart.h,28 :: 		while (RCIF_bit == 0)
L_RXCHR_USART2:
	BTFSC      RCIF_bit+0, BitPos(RCIF_bit+0)
	GOTO       L_RXCHR_USART3
;usart.h,29 :: 		; // espera pelo flag de interrupção de recepção
	GOTO       L_RXCHR_USART2
L_RXCHR_USART3:
;usart.h,30 :: 		RCIF_bit = 0;
	BCF        RCIF_bit+0, BitPos(RCIF_bit+0)
;usart.h,31 :: 		dado = RCREG;
	MOVF       RCREG+0, 0
	MOVWF      R1+0
;usart.h,32 :: 		if (OERR_bit) // Se acontecer algum erro de overflow
	BTFSS      OERR_bit+0, BitPos(OERR_bit+0)
	GOTO       L_RXCHR_USART4
;usart.h,34 :: 		CREN_bit = 0;
	BCF        CREN_bit+0, BitPos(CREN_bit+0)
;usart.h,36 :: 		CREN_bit = 1;
	BSF        CREN_bit+0, BitPos(CREN_bit+0)
;usart.h,37 :: 		}
L_RXCHR_USART4:
;usart.h,38 :: 		return dado; /*recebido no registrador RCREG e retorna para o programa principal*/
	MOVF       R1+0, 0
	MOVWF      R0+0
;usart.h,39 :: 		} // fim de função de recepção de caracteres
L_end_RXCHR_USART:
	RETURN
; end of _RXCHR_USART

_Dado_Pronto_USART:

;usart.h,42 :: 		char Dado_Pronto_USART()
;usart.h,44 :: 		return RCIF_bit; // retorna o bit RCIF (registrador PIR1, bit 5)
	MOVLW      0
	BTFSC      RCIF_bit+0, BitPos(RCIF_bit+0)
	MOVLW      1
	MOVWF      R0+0
;usart.h,45 :: 		} // fim da função Dado_Pronto_USART
L_end_Dado_Pronto_USART:
	RETURN
; end of _Dado_Pronto_USART

_TX_STRING_USART:

;usart.h,49 :: 		void TX_STRING_USART(const char *dado)
;usart.h,51 :: 		unsigned int i = 0;
	CLRF       TX_STRING_USART_i_L0+0
	CLRF       TX_STRING_USART_i_L0+1
;usart.h,52 :: 		while (dado[i] != '\0')
L_TX_STRING_USART5:
	MOVF       TX_STRING_USART_i_L0+0, 0
	ADDWF      FARG_TX_STRING_USART_dado+0, 0
	MOVWF      R0+0
	MOVF       FARG_TX_STRING_USART_dado+1, 0
	BTFSC      STATUS+0, 0
	ADDLW      1
	ADDWF      TX_STRING_USART_i_L0+1, 0
	MOVWF      R0+1
	MOVF       R0+0, 0
	MOVWF      ___DoICPAddr+0
	MOVF       R0+1, 0
	MOVWF      ___DoICPAddr+1
	CALL       _____DoICP+0
	MOVWF      R1+0
	MOVF       R1+0, 0
	XORLW      0
	BTFSC      STATUS+0, 2
	GOTO       L_TX_STRING_USART6
;usart.h,53 :: 		TXCHR_USART(dado[i++]);
	MOVF       TX_STRING_USART_i_L0+0, 0
	ADDWF      FARG_TX_STRING_USART_dado+0, 0
	MOVWF      R0+0
	MOVF       FARG_TX_STRING_USART_dado+1, 0
	BTFSC      STATUS+0, 0
	ADDLW      1
	ADDWF      TX_STRING_USART_i_L0+1, 0
	MOVWF      R0+1
	MOVF       R0+0, 0
	MOVWF      ___DoICPAddr+0
	MOVF       R0+1, 0
	MOVWF      ___DoICPAddr+1
	CALL       _____DoICP+0
	MOVWF      FARG_TXCHR_USART_dado+0
	CALL       _TXCHR_USART+0
	INCF       TX_STRING_USART_i_L0+0, 1
	BTFSC      STATUS+0, 2
	INCF       TX_STRING_USART_i_L0+1, 1
	GOTO       L_TX_STRING_USART5
L_TX_STRING_USART6:
;usart.h,54 :: 		} // fim da função TX_STRING_USART()
L_end_TX_STRING_USART:
	RETURN
; end of _TX_STRING_USART

_main:

;serialcon.c,3 :: 		void main()
;serialcon.c,6 :: 		unsigned char j = 0;
	CLRF       main_j_L0+0
;serialcon.c,9 :: 		ANSEL = ANSELH = 0;
	CLRF       ANSELH+0
	CLRF       ANSEL+0
;serialcon.c,10 :: 		C1ON_bit = C2ON_bit = 0; // Desabilita os comparadores
	BCF        C2ON_bit+0, BitPos(C2ON_bit+0)
	BTFSC      C2ON_bit+0, BitPos(C2ON_bit+0)
	GOTO       L__main23
	BCF        C1ON_bit+0, BitPos(C1ON_bit+0)
	GOTO       L__main24
L__main23:
	BSF        C1ON_bit+0, BitPos(C1ON_bit+0)
L__main24:
;serialcon.c,11 :: 		TRISB = 0;               // Porta B saída
	CLRF       TRISB+0
;serialcon.c,12 :: 		PORTB = j;
	MOVF       main_j_L0+0, 0
	MOVWF      PORTB+0
;serialcon.c,14 :: 		INICIAR_USART(9600); // Inicializa com 9600 bps
	MOVLW      128
	MOVWF      FARG_INICIAR_USART_Baud_Rate+0
	MOVLW      37
	MOVWF      FARG_INICIAR_USART_Baud_Rate+1
	CLRF       FARG_INICIAR_USART_Baud_Rate+2
	CLRF       FARG_INICIAR_USART_Baud_Rate+3
	CALL       _INICIAR_USART+0
;serialcon.c,15 :: 		Delay_ms(100);
	MOVLW      3
	MOVWF      R11+0
	MOVLW      138
	MOVWF      R12+0
	MOVLW      85
	MOVWF      R13+0
L_main7:
	DECFSZ     R13+0, 1
	GOTO       L_main7
	DECFSZ     R12+0, 1
	GOTO       L_main7
	DECFSZ     R11+0, 1
	GOTO       L_main7
	NOP
	NOP
;serialcon.c,17 :: 		while (1)
L_main8:
;serialcon.c,20 :: 		if (Dado_Pronto_USART())
	CALL       _Dado_Pronto_USART+0
	MOVF       R0+0, 0
	BTFSC      STATUS+0, 2
	GOTO       L_main10
;serialcon.c,23 :: 		recebe = RXCHR_USART();
	CALL       _RXCHR_USART+0
	MOVF       R0+0, 0
	MOVWF      main_recebe_L0+0
;serialcon.c,25 :: 		if (recebe == 'U')
	MOVF       R0+0, 0
	XORLW      85
	BTFSS      STATUS+0, 2
	GOTO       L_main11
;serialcon.c,27 :: 		j <<= 1;
	MOVF       main_j_L0+0, 0
	MOVWF      R1+0
	RLF        R1+0, 1
	BCF        R1+0, 0
	MOVF       R1+0, 0
	MOVWF      main_j_L0+0
;serialcon.c,28 :: 		if (j == 0)
	MOVF       R1+0, 0
	XORLW      0
	BTFSS      STATUS+0, 2
	GOTO       L_main12
;serialcon.c,29 :: 		j = 1;
	MOVLW      1
	MOVWF      main_j_L0+0
L_main12:
;serialcon.c,30 :: 		}
L_main11:
;serialcon.c,32 :: 		if (recebe == 'D')
	MOVF       main_recebe_L0+0, 0
	XORLW      68
	BTFSS      STATUS+0, 2
	GOTO       L_main13
;serialcon.c,34 :: 		j >>= 1;
	MOVF       main_j_L0+0, 0
	MOVWF      R1+0
	RRF        R1+0, 1
	BCF        R1+0, 7
	MOVF       R1+0, 0
	MOVWF      main_j_L0+0
;serialcon.c,35 :: 		if (j == 0)
	MOVF       R1+0, 0
	XORLW      0
	BTFSS      STATUS+0, 2
	GOTO       L_main14
;serialcon.c,36 :: 		j = 128;
	MOVLW      128
	MOVWF      main_j_L0+0
L_main14:
;serialcon.c,37 :: 		}
L_main13:
;serialcon.c,39 :: 		PORTB = j;
	MOVF       main_j_L0+0, 0
	MOVWF      PORTB+0
;serialcon.c,40 :: 		} // fim do if
L_main10:
;serialcon.c,41 :: 		} // fim do while
	GOTO       L_main8
;serialcon.c,42 :: 		} // fim do main
L_end_main:
	GOTO       $+0
; end of _main
