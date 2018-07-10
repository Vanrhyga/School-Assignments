;����
C8251PORTIO     EQU 2B8H
C8251PORTCTRL   EQU 2B9H
C8254PORT0      EQU 280H
C8254PORT1      EQU 281H
C8254PORTCTRL   EQU 283H
C8255PORTA      EQU 288H
C8255PORTB      EQU 289H
C8255PORTC      EQU 28AH
C8255PORTCTRL   EQU 28BH
DISPLOC         EQU 93H

;���ݶ�
DATA    SEGMENT
    KEY     DB  71H,79H,5EH,39H,7CH,77H,6FH,7FH,07H,7DH,6DH,66H,4FH,5BH,06H,3FH ;�����
    BUFFER  DB            
DATA    ENDS

;�����
CODE    SEGMENT
    ASSUME CS:CODE,DS:DATA
START:
    MOV AX,DATA
    MOV DS,AX       ;DSָ��DATA��
    
;��ʼ��8251
    MOV DX,C8251PORTCTRL
    XOR AL,AL
    OUT DX,AL       ;Ԥ��ʼ��
    MOV AL,40H
    OUT DX,AL       ;��λ
    NOP             ;��ʱ
    
    MOV AL,11001111B;��ʽ�����֣��첽��ʽ��2bitֹͣλ����У�飬8bit����λ��64������
    MOV DX,C8251CTRL
    OUT DX,AL
    
    MOV AL,00110111B;���������֣����������������ͣ���λ�����־�����ܷ�������
    OUT DX,AL
    
;��ʼ��8254
    MOV AL,00110100B;�����֣�������0���ȵͺ�ߣ���ʽ2�������Ƽ���
    MOV DX,C8254PORTCTRL
    OUT DX,AL
    
    MOV AX,28H      ;������ֵ40
    MOV DX,C8254PORT0
    OUT DX,AL       ;д�������ֵ���ֽ�
    MOV AL,AH
    OUT DX,AL       ;д�������ֵ���ֽ�
    
;��ʼ��8255
    MOV AL,10000001B;ѡ������֣�A�鷽ʽ0��A�������C�ڸ���λ�����B�鷽ʽ0��B�������C�����λ����
    MOV DX,C8255PORTCTRL
    OUT DX,AL
    
;ѭ��ɨ�����
INPUT:
    MOV AL,00H
    MOV DX,C8255PORTC
    OUT DX,AL
   
SCAN:
    MOV AL,00H
    MOV DX,C8255PORTB
    OUT DX,AL
    
    CALL   DISPLAY
    MOV DX,C8255PORTC
    IN  AL,DX
;------------�������ʾ------------
DISPLAY PROC
    PUSH    CX
    PUSH    AX
    
    MOV AL,00H
    MOV DX,C8255PORTB
    OUT DX,AL
    MOV AL,BUFFER[0]
    AND AX,00FFH    ;ȡ��λ����  
    MOV SI,AX
    MOV AL,KEY[SI]
    MOV DX,C8255PORTA
    OUT DX,AL       ;���͵�һ�����������
    MOV AL,10H
    MOV DX,C8255PORTB
    OUT DX,AL       ;���͵�һ�������λ���źţ�����
    
    CALL    DELAY1
                  
    MOV AL,00H
    MOV DX,C8255PORTB
    OUT DX,AL
    MOV AL,BUFFER[1]
    AND AX,00FFH
    MOV SI,AX
    MOV AL,KEY[SI]
    MOV DX,C8255PORTA
    OUT DX,AL       ;���͵ڶ������������
    MOV AL,20H
    MOV DX,C8255PORTB
    OUT DX,AL       ;���͵ڶ��������λ���źţ�����
    
    CALL    DELAY1
    
    MOV AL,00H
    MOV DX,C8255PORTB
    OUT DX,AL
    MOV AL,BUFFER[2]
    AND AX,00FFH
    MOV SI,AX
    MOV AL,KEY[SI]
    MOV DX,C8255PORTA
    OUT DX,AL       ;���͵��������������
    MOV AL,40H
    MOV DX,C8255PORTB
    OUT DX,AL       ;���͵����������λ���źţ�����
    
    CALL    DELAY1
    
    MOV AL,00H
    MOV DX,C8255PORTB
    MOV DX,AL
    MOV AL,BUFFER[3]
    AND AX,00FFH
    MOV SI,AX
    MOV AL,KEY[SI]
    MOV DX,C8255PORTA
    OUT DX,AL   ;���͵��ĸ����������
    MOV AL,80H
    MOV DX,C8255PORTB
    OUT DX,AL   ;���͵��ĸ������λ���źţ�����
    
    POP AX
    POP CX
    RET    
DISPLAY ENDP    
;------------�ӳ�------------
DELAY   PROC
    PUSH    CX
    MOV CX,0FFFH
LP1:
    LOOP    LP1
    POP CX
    RET 
DELAY   ENDP

DELAY1  PROC
    PUSH    CX
    MOV CX,24H
LP2:
    CALL    DELAY
    LOOP    LP2
    POP CX
    RET 
DELAY1 ENDP         