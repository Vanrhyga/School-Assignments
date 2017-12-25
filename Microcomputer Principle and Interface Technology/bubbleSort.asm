DATA    SEGMENT                         ;数据段
    MSG1    DB   'Please enter positive integers less than 128, and separate them by Spaces. '
            DB   0DH,0AH,'End the input with a line break:',0DH,0AH,24H 
    MSG2    DB   0DH,0AH,0DH,0AH,'The bubble sort result is:',0DH,0AH,24H
    ARRAY   DB   50 DUP(0)              ;待排序数据
    N       DW   0                      ;实际数据个数
    TMP     DW   0
DATA    ENDS

CODE    SEGMENT                         ;代码段
    ASSUME  CS:CODE,DS:DATA             ;关联寄存器
START:
    MOV     AX,DATA                     ;初始化
    MOV     DS,AX
    CALL    INPUT                       ;调用输入子程序
    CALL    SORT                        ;调用排序子程序
    CALL    OUTPUT                      ;调用输出子程序
    MOV     AH,4CH                      ;控制返回操作系统
    INT     21H  
    
INPUT   PROC    NEAR                    ;输入子程序
    LEA     DX,MSG1                     ;输出提示信息
    MOV     AH,09H
    INT     21H  
    LEA     DI,ARRAY   
    MOV     BX,0   
    JMP     NEWCHAR
NEWNUMBER:  
    MOV     [DI],BL 
    INC     DI   
    MOV     BX,N                        ;数据个数增加
    INC     BX
    MOV     N,BX
    CMP     AL,20H-30H                  ;既非数字，也非空格，标志输入结束
    JNZ     EXIT
    MOV     BX,0                        ;十进制数据转换为二进制存储
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
              
SORT    PROC    NEAR                    ;排序子程序
    MOV     CX,N
    DEC     CX
L1:
    MOV     DI,CX                       ;暂存外循环计数值
    XOR     BX,BX
L2:
    MOV     AL,ARRAY[BX]
    CMP     AL,ARRAY[BX+1]
    JLE     CONTINUE
    XCHG    AL,ARRAY[BX+1]              ;大于则交换
    MOV     ARRAY[BX],AL
CONTINUE:
    INC     BX
    LOOP    L2                          ;内循环
    MOV     CX,DI
    LOOP    L1                          ;外循环，计数值递减
    RET
SORT    ENDP              
              
OUTPUT  PROC    NEAR                    ;输出子程序
    LEA     DX,MSG2                     ;输出提示信息
    MOV     AH,09H
    INT     21H   
    LEA     SI,ARRAY
    DEC     SI 
    MOV     DI,N
NEXT:
    MOV     AX,TMP 
    CMP     AX,10
    JNZ     NEXTNUMBER   
    MOV     DX,0DH                      ;已输出10个数据，换行
    MOV     AH,02H
    INT     21H 
    MOV     DX,0AH
    INT     21H 
    MOV     AX,0
NEXTNUMBER:
    INC     AX
    MOV     TMP,AX
    MOV     CX,3                        ;二进制排序结果转换为十进制输出
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