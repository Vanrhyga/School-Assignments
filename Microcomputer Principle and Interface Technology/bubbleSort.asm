DATA    SEGMENT                         ;���ݶ�
    MSG1    DB   'Please enter positive integers less than 128, and separate them by Spaces. '
            DB   0DH,0AH,'End the input with a line break:',0DH,0AH,24H 
    MSG2    DB   0DH,0AH,0DH,0AH,'The bubble sort result is:',0DH,0AH,24H
    ARRAY   DB   50 DUP(0)              ;����������
    N       DW   0                      ;ʵ�����ݸ���
    TMP     DW   0
DATA    ENDS

CODE    SEGMENT                         ;�����
    ASSUME  CS:CODE,DS:DATA             ;�����Ĵ���
START:
    MOV     AX,DATA                     ;��ʼ��
    MOV     DS,AX
    CALL    INPUT                       ;���������ӳ���
    CALL    SORT                        ;���������ӳ���
    CALL    OUTPUT                      ;��������ӳ���
    MOV     AH,4CH                      ;���Ʒ��ز���ϵͳ
    INT     21H  
    
INPUT   PROC    NEAR                    ;�����ӳ���
    LEA     DX,MSG1                     ;�����ʾ��Ϣ
    MOV     AH,09H
    INT     21H  
    LEA     DI,ARRAY   
    MOV     BX,0   
    JMP     NEWCHAR
NEWNUMBER:  
    MOV     [DI],BL 
    INC     DI   
    MOV     BX,N                        ;���ݸ�������
    INC     BX
    MOV     N,BX
    CMP     AL,20H-30H                  ;�ȷ����֣�Ҳ�ǿո񣬱�־�������
    JNZ     EXIT
    MOV     BX,0                        ;ʮ��������ת��Ϊ�����ƴ洢
NEWCHAR:
    MOV     AH,01H
    INT     21H 
    SUB     AL,30H
    JL      NEWNUMBER
    CMP     AL,9D
    JG      NEWNUMBER
    CBW
    XCHG    AX,BX
    MOV     CX,10D
    MUL     CX
    XCHG    AX,BX
    ADD     BX,AX
    JMP     NEWCHAR
EXIT:       
    RET              
INPUT   ENDP  
              
SORT    PROC    NEAR                    ;�����ӳ���
    MOV     CX,N
    DEC     CX
L1:
    MOV     DI,CX                       ;�ݴ���ѭ������ֵ
    XOR     BX,BX
L2:
    MOV     AL,ARRAY[BX]
    CMP     AL,ARRAY[BX+1]
    JLE     CONTINUE
    XCHG    AL,ARRAY[BX+1]              ;�����򽻻�
    MOV     ARRAY[BX],AL
CONTINUE:
    INC     BX
    LOOP    L2                          ;��ѭ��
    MOV     CX,DI
    LOOP    L1                          ;��ѭ��������ֵ�ݼ�
    RET
SORT    ENDP              
              
OUTPUT  PROC    NEAR                    ;����ӳ���
    LEA     DX,MSG2                     ;�����ʾ��Ϣ
    MOV     AH,09H
    INT     21H   
    LEA     SI,ARRAY
    DEC     SI 
    MOV     DI,N
NEXT:
    MOV     AX,TMP 
    CMP     AX,10
    JNZ     NEXTNUMBER   
    MOV     DX,0DH                      ;�����10�����ݣ�����
    MOV     AH,02H
    INT     21H 
    MOV     DX,0AH
    INT     21H 
    MOV     AX,0
NEXTNUMBER:
    INC     AX
    MOV     TMP,AX
    MOV     CX,3                        ;������������ת��Ϊʮ�������
    INC     SI
    MOV     AL,[SI] 
    CBW  
    MOV     BX,10
LP1:
    XOR     DX,DX
    DIV     BX
    ADD     DL,30H
    PUSH    DX
    LOOP    LP1
    MOV     CX,3
LP2:
    POP     DX
    MOV     AH,02H
    INT     21H 
    LOOP    LP2     
    MOV     DX,20H
    MOV     AH,02H
    INT     21H 
    DEC     DI
    CMP     DI,0
    JNZ     NEXT 
    MOV     DX,0DH
    MOV     AH,02H
    INT     21H 
    MOV     DX,0AH
    INT     21H
    RET
OUTPUT  ENDP
CODE    ENDS
END     START