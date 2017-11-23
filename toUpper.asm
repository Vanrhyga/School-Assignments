DATA    SEGMENT                              ;数据段
    BUFFER  DB 50                            ;最大输入字符数
            DB 0                             ;实际输入字符数
    STR     DB 50 DUP(0)                     ;输入字符串
    TIP1    DB 'Please enter a string that needs to be converted to uppercase:',0DH,0AH,'$'
    TIP2    DB 'The result is:',0DH,0AH,'$'
DATA    ENDS

CODE    SEGMENT                              ;代码段
    ASSUME      CS:CODE,DS:DATA              ;关联寄存器
START:
    MOV     AX,DATA                          ;初始化
    MOV     DS,AX
    MOV     AH,09H                           ;输出提示信息
    LEA     DX,TIP1
    INT     21H
    MOV     AH,0AH                           ;从标准输入流读字符串
    LEA     DX,BUFFER
    INT     21H
    INC     DX
    MOV     SI,DX
    MOV     CL,BYTE PTR [SI]                 ;CX置为字符串长度
    XOR     CH,CH
    ADD     SI,2   
    ADD     SI,CX
    MOV     BYTE PTR [SI],'$'                ;字符串末尾增添'$'
    LEA     SI,STR
CONTINUE:                                    ;判断是否为小写字母，并进行相应转换    
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
    LEA     DX,STR                           ;输出结果
    INT     21H 
    MOV     AH,4CH                           ;控制返回操作系统
    INT     21H
CODE    ENDS
END     START