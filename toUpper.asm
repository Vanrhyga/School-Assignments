DATA    SEGMENT                              ;���ݶ�
    BUFFER  DB 50                            ;��������ַ���
            DB 0                             ;ʵ�������ַ���
    STR     DB 50 DUP(0)                     ;�����ַ���
    TIP1    DB 'Please enter a string that needs to be converted to uppercase:',0DH,0AH,'$'
    TIP2    DB 'The result is:',0DH,0AH,'$'
DATA    ENDS

CODE    SEGMENT                              ;�����
    ASSUME      CS:CODE,DS:DATA              ;�����Ĵ���
START:
    MOV     AX,DATA                          ;��ʼ��
    MOV     DS,AX
    MOV     AH,09H                           ;�����ʾ��Ϣ
    LEA     DX,TIP1
    INT     21H
    MOV     AH,0AH                           ;�ӱ�׼���������ַ���
    LEA     DX,BUFFER
    INT     21H
    INC     DX
    MOV     SI,DX
    MOV     CL,BYTE PTR [SI]                 ;CX��Ϊ�ַ�������
    XOR     CH,CH
    ADD     SI,2   
    ADD     SI,CX
    MOV     BYTE PTR [SI],'$'                ;�ַ���ĩβ����'$'
    LEA     SI,STR
CONTINUE:                                    ;�ж��Ƿ�ΪСд��ĸ����������Ӧת��    
    CMP     [SI],61H
    JB      NEXT
    CMP     [SI],7AH
    JA      NEXT
    SUB     [SI],20H     
NEXT:
    INC     SI
    LOOP    CONTINUE
    MOV     AH,02H
    MOV     DL,0DH
    INT     21H 
    MOV     DL,0AH
    INT     21H   
    MOV     DL,0DH
    INT     21H 
    MOV     DL,0AH
    INT     21H
    MOV     AH,09H
    LEA     DX,TIP2
    INT     21H                          
    LEA     DX,STR                           ;������
    INT     21H 
    MOV     AH,4CH                           ;���Ʒ��ز���ϵͳ
    INT     21H
CODE    ENDS
END     START