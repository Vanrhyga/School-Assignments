;����
C8251PORTIO     EQU 2B8H    ;8251 IO�˿�
C8251PORTCTRL   EQU 2B9H    ;8251 ���ƶ˿�
C8254PORT0      EQU 280H    ;8254 �˿�0
C8254PORTCTRL   EQU 283H    ;8254 ���ƶ˿�
C8255PORTA      EQU 288H    ;8255 A��
C8255PORTB      EQU 289H    ;8255 B��
C8255PORTC      EQU 28AH    ;8255 C��
C8255PORTCTRL   EQU 28BH    ;8255 ���ƶ˿�

;���ݶ�
DATA    SEGMENT
    KEY     DB  71H,79H,5EH,39H,7CH,77H,6FH,7FH,07H,7DH,6DH,66H,4FH,5BH,06H,3FH ;�����
    ASCII   DB  46H,45H,44H,43H,42H,41H,39H,38H,37H,36H,35H,34H,33H,32H,31H,30H ;ASCII��
    BUFFER1 DB  00H,00H,00H,00H 
    BUFFER2 DB  00H,00H,00H,00H    
    LCD_END DB  45H,4EH,44H ;END
    LINEKEY DB  ?
DATA    ENDS

;�����
CODE    SEGMENT
    ASSUME CS:CODE,DS:DATA
    
START:
    MOV AX,DATA
    MOV DS,AX               ;DSָ�����ݶ�
    
;��ʼ��8251
    MOV DX,C8251PORTCTRL
    XOR AL,AL
    OUT DX,AL               ;Ԥ��ʼ��
    MOV AL,40H
    OUT DX,AL               ;��λ 
    
    NOP                     ;��ʱ
    
    MOV AL,01001111B        ;��ʽ�����֣��첽��ʽ��1bitֹͣλ����У�飬8bit����λ������������64
    MOV DX,C8251PORTCTRL
    OUT DX,AL
    
    MOV AL,00100111B        ;���������֣����������������ͣ����ܷ�������
    OUT DX,AL
    
;��ʼ��8254
    MOV AL,00010100B        ;�����֣�������0�����ֽڣ���ʽ2�������Ƽ���
    MOV DX,C8254PORTCTRL
    OUT DX,AL
    
    MOV AX,24H              ;������ֵ
    MOV DX,C8254PORT0
    OUT DX,AL      
    
;��ʼ��8255
    MOV AL,10000001B        ;��ʽѡ������֣�A�鷽ʽ0��A�������C�ڸ���λ�����B�鷽ʽ0��B�������C�����λ����
    MOV DX,C8255PORTCTRL
    OUT DX,AL

;��ʼ��Һ����
    CALL CLEAR              ;����
    
    CALL DISPLAYONLCD       ;��ʾ
    
;ѭ��ɨ����̼�8251״̬��
INPUT:
    MOV AL,00H
    MOV DX,C8255PORTC
    OUT DX,AL
   
SCAN1:
    MOV AL,00H
    MOV DX,C8255PORTB
    OUT DX,AL
    
    CALL DISPLAY            ;�������ʾ
    CALL DISPLAYONLCD       ;LCD��ʾ   
    
    MOV DX,C8255PORTC
    IN  AL,DX               ;������ֵ   
    AND AL,0FH              ;ȡ��4λ
    CMP AL,0FH
    JZ  SCAN2               ;�޼�����          
              	
    CALL DELAY2    

    MOV BL,4                ;����
    MOV BH,4                ;����
    MOV AL,0EFH             ;ɨ����  
    MOV CH,0FFH             ;С�����±�ֵ     
    
ROW:
    MOV DX,C8255PORTC
    OUT DX,AL               ;C�ڸ���λ���ɨ����     
    ROL AL,1                ;ѭ������������һɨ����
    MOV AH,AL               ;������һɨ����
    
    MOV DX,C8255PORTC
    IN  AL,DX               ;C�ڵ���λ������ֵ
    MOV LINEKEY,AL           
    AND AL,0FH              ;ȡ����λ
    CMP AL,0FH              ;�ж��Ƿ�������Ϊ0
    JNZ HANDLE              ;�м�����
    
    ADD CH,BH               ;�±�����
    MOV AL,AH               ;ȡɨ����
    DEC BL                  ;�м���-1
    JNZ ROW                 ;������Ϊ0������ɨ��
    
    JMP SCAN2
        
HANDLE:
    MOV DX,C8255PORTC
    CALL DELAY
    IN  AL,DX
    CMP AL,LINEKEY
    JZ  HANDLE              ;����δ̧��
    
    MOV AL,LINEKEY
    
COL:
    INC CH                  ;�±�ֵ����
    RCR AL,1
    JC  COL                 ;δ����
    
    MOV AL,CH               ;�����±�ֵ
   
    CALL DISPLAYONSCREEN    ;��Ļ��ʾ

    CALL MOVE1              ;��������

;8251����
    CMP AL,01H  
    JZ  EXIT                ;����E���˳�

    PUSH AX
    MOV DX,C8251PORTCTRL
    IN  AL,DX
    AND AL,01H               
    POP AX
    JZ  SCAN2               ;������δ׼����

    MOV DX,C8251PORTIO
    OUT DX,AL               ;���ͼ�ֵ 

SCAN2:
    MOV DX,C8251PORTCTRL
    IN  AL,DX               ;��״̬��
    AND AL,02H  
    JZ  INPUT               ;������δ׼����

    MOV DX,C8251PORTIO
    IN  AL,DX               ;��������

    CALL MOVE2              ;��������

    JMP INPUT   
    
EXIT:        
    CALL LCD_EXIT           ;LCD�˳�
    MOV AX,4C00H
    INT 21H                 ;����DOS     
;------------�������ʾ------------
DISPLAY PROC
    PUSH CX
    PUSH AX
    PUSH DX
    PUSH SI
    
    MOV AL,00H
    MOV DX,C8255PORTB
    OUT DX,AL
    MOV AL,BUFFER1[0]
    AND AX,00FFH    
    MOV SI,AX
    MOV AL,KEY[SI]
    MOV DX,C8255PORTA
    OUT DX,AL               ;�����һ�����������
    MOV AL,80H
    MOV DX,C8255PORTB
    OUT DX,AL               ;�����һ�������λ���ź�
    
    CALL DELAY1
                  
    MOV AL,00H
    MOV DX,C8255PORTB
    OUT DX,AL
    MOV AL,BUFFER1[1]
    AND AX,00FFH
    MOV SI,AX
    MOV AL,KEY[SI]
    MOV DX,C8255PORTA
    OUT DX,AL               ;����ڶ������������
    MOV AL,40H
    MOV DX,C8255PORTB
    OUT DX,AL               ;����ڶ��������λ���ź�
    
    CALL DELAY1
    
    MOV AL,00H
    MOV DX,C8255PORTB
    OUT DX,AL
    MOV AL,BUFFER1[2]
    AND AX,00FFH
    MOV SI,AX
    MOV AL,KEY[SI]
    MOV DX,C8255PORTA
    OUT DX,AL               ;������������������
    MOV AL,20H
    MOV DX,C8255PORTB
    OUT DX,AL               ;��������������λ���ź�
    
    CALL DELAY1
    
    MOV AL,00H
    MOV DX,C8255PORTB
    OUT DX,AL
    MOV AL,BUFFER1[3]
    AND AX,00FFH
    MOV SI,AX
    MOV AL,KEY[SI]
    MOV DX,C8255PORTA
    OUT DX,AL               ;������ĸ����������
    MOV AL,10H

    MOV DX,C8255PORTB
    OUT DX,AL               ;������ĸ������λ���ź�

    CALL DELAY1

    MOV AL,00H
    MOV DX,C8255PORTB
    OUT DX,AL
    
    POP SI
    POP DX
    POP AX
    POP CX
    RET    
DISPLAY ENDP    
;------------��Ļ��ʾ------------
DISPLAYONSCREEN PROC
    PUSH AX
    
    AND AX,0FH
    MOV SI,AX
    MOV DL,ASCII[SI]        ;������ӦASCII��   
    MOV AH,02H
    INT 21H

    CALL DISPLAY            ;�������ʾ

    POP AX
    RET
DISPLAYONSCREEN ENDP   
;------------Һ������ʾ------------
DISPLAYONLCD PROC
    PUSH DX
    PUSH AX
    PUSH SI
    
;��ʾ��һ������
    MOV DX,C8255PORTA
    MOV AL,96H
    OUT DX,AL               ;�趨��ʾλ��
    
    CALL LCD_CTRL           ;����ָ��
    
    MOV DX,C8255PORTA
    MOV AL,BUFFER2[0]
    AND AX,0FFH
    MOV SI,AX
    MOV AL,ASCII[SI]
    OUT DX,AL
    
    CALL LCD_DATA           ;��������    
    
    CALL DELAY
    
;��ʾ�ڶ�������
    MOV DX,C8255PORTA
    MOV AL,94H  
    OUT DX,AL               ;�趨��ʾλ��
    
    CALL LCD_CTRL           ;����ָ��
    
    MOV DX,C8255PORTA
    MOV AL,BUFFER2[1]
    AND AX,0FFH
    MOV SI,AX
    MOV AL,ASCII[SI]
    OUT DX,AL
    
    CALL LCD_DATA           ;��������
    
    CALL DELAY
    
;��ʾ����������
    MOV DX,C8255PORTA
    MOV AL,92H
    OUT DX,AL               ;�趨��ʾλ��
    
    CALL LCD_CTRL           ;����ָ��
    
    MOV DX,C8255PORTA
    MOV AL,BUFFER2[2]
    AND AX,0FFH
    MOV SI,AX
    MOV AL,ASCII[SI]
    OUT DX,AL   
    
    CALL LCD_DATA           ;��������
    
    CALL DELAY
             
;��ʾ���ĸ�����
    MOV DX,C8255PORTA
    MOV AL,90H
    OUT DX,AL               ;�趨��ʾλ��
    
    CALL LCD_CTRL           ;����ָ��
    
    MOV DX,C8255PORTA
    MOV AL,BUFFER2[3]
    AND AX,0FFH
    MOV SI,AX
    MOV AL,ASCII[SI]
    OUT DX,AL   
    
    CALL LCD_DATA           ;��������
    
    CALL DELAY
    
    POP SI
    POP AX
    POP DX               
    RET               
DISPLAYONLCD    ENDP    
;------------Һ��������ָ��------------
LCD_CTRL    PROC
    MOV DX,C8255PORTB   
    MOV AL,00000000B        ;дָ��
    OUT DX,AL
    
    CALL DELAY

;����    
    MOV AL,00000100B
    OUT DX,AL
    
    CALL DELAY
    
    MOV AL,00000000B
    OUT DX,AL
    
    CALL DELAY

    CALL DISPLAY            ;�������ʾ
    
    RET   
LCD_CTRL    ENDP       
;------------Һ������������------------
LCD_DATA    PROC
    MOV DX,C8255PORTB
    
    MOV AL,00000001B        ;д����
    OUT DX,AL
    
    CALL DELAY

;����    
    MOV AL,00000101B
    OUT DX,AL
    
    CALL DELAY
    
    MOV AL,00000001B
    OUT DX,AL
    
    CALL DELAY
    
    RET   
LCD_DATA    ENDP
;------------Һ��������------------
CLEAR   PROC
    MOV DX,C8255PORTA
    MOV AL,00001100B    
    OUT DX,AL               ;������ʾ
    
    CALL LCD_CTRL           ;����ָ��
    
    MOV DX,C8255PORTA
    MOV AL,00000001B    
    OUT DX,AL               ;����
    
    CALL LCD_CTRL           ;����ָ��
                      
    RET      
CLEAR   ENDP                 
;------------Һ�����˳�------------
LCD_EXIT    PROC
    CALL CLEAR
    
    MOV SI,0
    MOV BL,92H              ;�趨��ʾλ��
    MOV CX,3
    
LP:
    MOV DX,C8255PORTA
    MOV AL,BL
    OUT DX,AL               ;�趨��ʾλ��
    
    CALL LCD_CTRL           ;����ָ��
    
    MOV DX,C8255PORTA
    MOV AL,LCD_END[SI]
    OUT DX,AL
    
    CALL LCD_DATA           ;��������
    
    INC SI
    INC BL
    LOOP LP
    
    RET 
LCD_EXIT    ENDP
;------------��λ------------
MOVE1	PROC
    PUSH AX
    
    MOV AL,BUFFER1[2]
    MOV BUFFER1[3],AL
    MOV AL,BUFFER1[1]
    MOV BUFFER1[2],AL
    MOV AL,BUFFER1[0]
    MOV BUFFER1[1],AL
    
    POP AX
    
    MOV BUFFER1[0],AL        ;�����ݷ���BUFFER1[0]��

    RET
MOVE1	ENDP

MOVE2	PROC
    PUSH AX
    
    MOV AL,BUFFER2[2]
    MOV BUFFER2[3],AL
    MOV AL,BUFFER2[1]
    MOV BUFFER2[2],AL
    MOV AL,BUFFER2[0]
    MOV BUFFER2[1],AL
    
    POP AX
    
    MOV BUFFER2[0],AL        ;�����ݷ���BUFFER2[0]��

    RET
MOVE2	ENDP
;------------�ӳ�------------
DELAY   PROC
    PUSH CX
    
    MOV CX,0FFFH
LP1:
    LOOP LP1
    
    POP CX
    RET 
DELAY   ENDP

DELAY1  PROC
    PUSH CX
    
    MOV CX,24H
LP2:
    CALL DELAY
    LOOP LP2
    
    POP CX
    RET 
DELAY1 ENDP         

DELAY2  PROC
    PUSH CX
    
    MOV CX,60H
LP3:
    CALL DELAY
    LOOP LP3
    
    POP CX
    RET 
DELAY2 ENDP 

CODE    ENDS
    END START