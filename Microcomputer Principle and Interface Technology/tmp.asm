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
    ASCII   DB  46H,45H,44H,43H,42H,41H,39H,38H,37H,36H,35H,34H,33H,32H,31H,30H ;ASCII��
    BUFFER  DB  00H,00H,00H,00H
    LINEKEY DB  ? 
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
    
    MOV AL,01101111B;��ʽ�����֣��첽��ʽ��2bitֹͣλ����У�飬8bit����λ������������64
    MOV DX,C8251PORTCTRL
    OUT DX,AL
    
    MOV AL,00100111B;���������֣����������������ͣ���λ�����־�����ܷ�������
    OUT DX,AL
    
;��ʼ��8254
    MOV AL,00010110B;�����֣�������0�����ֽڣ���ʽ3�������Ƽ���
    MOV DX,C8254PORTCTRL
    OUT DX,AL
    
    MOV AX,24H      ;������ֵ
    MOV DX,C8254PORT0
    OUT DX,AL       ;д�������ֵ
    
;��ʼ��8255
    MOV AL,10000001B;ѡ������֣�A�鷽ʽ0��A�������C�ڸ���λ�����B�鷽ʽ0��B�������C�����λ����
    MOV DX,C8255PORTCTRL
    OUT DX,AL
    
;ѭ��ɨ����̼�8251״̬��
INPUT:
    MOV AL,00H
    MOV DX,C8255PORTC
    OUT DX,AL
   
SCAN1:
    MOV AL,00H
    MOV DX,C8255PORTB
    OUT DX,AL
    
    CALL   DISPLAY
    MOV DX,C8255PORTC
    IN  AL,DX   ;������ֵ   
    AND AL,0FH  ;����4λ
    CMP AL,0FH
    
    JZ  SCAN2   ;�޼�����          
              	
    CALL	DELAY2    

    MOV BL,4    ;����
    MOV BH,4    ;����
    MOV AL,0EFH ;ɨ����  
    MOV CH,0FFH     
    
ROW:
    MOV DX,C8255PORTC
    OUT DX,AL   ;C�ڸ���λ���ɨ����     
    ROL AL,1    ;ѭ������������һɨ����
    MOV AH,AL   ;������һɨ����
    MOV DX,C8255PORTC
    IN  AL,DX   ;C����λ������ֵ
    MOV LINEKEY,AL           
    AND AL,0FH ;����λ
    CMP AL,0FH  ;�ж��Ƿ�������Ϊ0
    JNZ HANDLE     
    ADD CH,BH
    MOV AL,AH   ;ȡɨ����
    DEC BL      ;�м���-1
    JNZ ROW     ;������Ϊ0������ɨ��
    
    JMP SCAN2
        
HANDLE:
    MOV DX,C8255PORTC
    CALL    DELAY
    IN  AL,DX
    CMP AL,LINEKEY
    JZ  HANDLE
    MOV AL,LINEKEY
    
COL:
    INC CH
    RCR AL,1
    JC  COL ;���λΪ1��δ���£�����ѭ��
    
    MOV AL,CH   ;������±�ֵ

    CALL MOVE

;8251����
    CMP AL,01H  ;����Eʱ�˳�
    JZ  EXIT

    PUSH AX

    MOV DX,C8251PORTCTRL
    IN AL,DX
    AND AL,01H  
    POP AX
    JZ SCAN2

    MOV DX,C8251PORTIO
    OUT DX,AL   ;���ͼ�ֵ 

SCAN2:
    MOV DX,C8251PORTCTRL
    IN  AL,DX   ;��״̬��
    AND AL,02H  ;�ж�RxRDY=1?

    JZ  INPUT

    MOV DX,C8251PORTIO
    IN  AL,DX   ;��������

    CALL MOVE

    JMP INPUT
EXIT:
    MOV AX,4C00H
    INT 21H ;����DOS     
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
    MOV AL,80H
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
    MOV AL,40H
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
    MOV AL,20H
    MOV DX,C8255PORTB
    OUT DX,AL       ;���͵����������λ���źţ�����
    
    CALL    DELAY1
    
    MOV AL,00H
    MOV DX,C8255PORTB
    OUT DX,AL
    MOV AL,BUFFER[3]
    AND AX,00FFH
    MOV SI,AX
    MOV AL,KEY[SI]
    MOV DX,C8255PORTA
    OUT DX,AL   ;���͵��ĸ����������
    MOV AL,10H

    MOV DX,C8255PORTB
    OUT DX,AL   ;���͵��ĸ������λ���źţ�����

    CALL DELAY1
    MOV AL,00H
    MOV DX,C8255PORTB
    OUT DX,AL
    
    POP AX
    POP CX
    RET    
DISPLAY ENDP   
;------------��λ------------
MOVE	PROC
    PUSH    AX
    MOV AL,BUFFER[2]
    MOV BUFFER[3],AL
    MOV AL,BUFFER[1]
    MOV BUFFER[2],AL
    MOV AL,BUFFER[0]
    MOV BUFFER[1],AL
    POP AX
    MOV BUFFER[0],AL    ;�����ݷ���BUFFER[0]��
MOVE	ENDP
;------------��Ļ��ʾ------------
DISPLAYONSCREEN PROC
    PUSH AX
    AND AX,0FH
    MOV SI,AX
    MOV DL,ASCII[SI]  ;������ӦASCII��   
    MOV AH,02H
    INT 21H
    POP AX
DISPLAYONSCREEN ENDP
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

DELAY2  PROC
    PUSH    CX
    MOV CX,60H
LP3:
    CALL    DELAY
    LOOP    LP3
    POP CX
    RET 
DELAY2 ENDP 

CODE    ENDS
    END START