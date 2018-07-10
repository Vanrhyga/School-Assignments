;常量
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

;数据段
DATA    SEGMENT
    KEY     DB  71H,79H,5EH,39H,7CH,77H,6FH,7FH,07H,7DH,6DH,66H,4FH,5BH,06H,3FH ;数码管
    BUFFER  DB            
DATA    ENDS

;代码段
CODE    SEGMENT
    ASSUME CS:CODE,DS:DATA
START:
    MOV AX,DATA
    MOV DS,AX       ;DS指向DATA段
    
;初始化8251
    MOV DX,C8251PORTCTRL
    XOR AL,AL
    OUT DX,AL       ;预初始化
    MOV AL,40H
    OUT DX,AL       ;软复位
    NOP             ;延时
    
    MOV AL,11001111B;方式命令字，异步方式，2bit停止位，无校验，8bit数据位，64波特率
    MOV DX,C8251CTRL
    OUT DX,AL
    
    MOV AL,00110111B;工作命令字，正常操作，请求发送，复位错误标志，接受发送允许
    OUT DX,AL
    
;初始化8254
    MOV AL,00110100B;控制字，计数器0，先低后高，方式2，二进制计数
    MOV DX,C8254PORTCTRL
    OUT DX,AL
    
    MOV AX,28H      ;计数初值40
    MOV DX,C8254PORT0
    OUT DX,AL       ;写入计数初值低字节
    MOV AL,AH
    OUT DX,AL       ;写入计数初值高字节
    
;初始化8255
    MOV AL,10000001B;选择控制字，A组方式0，A口输出，C口高四位输出，B组方式0，B口输出，C组第四位输入
    MOV DX,C8255PORTCTRL
    OUT DX,AL
    
;循环扫描键盘
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
;------------数码管显示------------
DISPLAY PROC
    PUSH    CX
    PUSH    AX
    
    MOV AL,00H
    MOV DX,C8255PORTB
    OUT DX,AL
    MOV AL,BUFFER[0]
    AND AX,00FFH    ;取低位数据  
    MOV SI,AX
    MOV AL,KEY[SI]
    MOV DX,C8255PORTA
    OUT DX,AL       ;传送第一个数码管数据
    MOV AL,10H
    MOV DX,C8255PORTB
    OUT DX,AL       ;传送第一个数码管位控信号，点亮
    
    CALL    DELAY1
                  
    MOV AL,00H
    MOV DX,C8255PORTB
    OUT DX,AL
    MOV AL,BUFFER[1]
    AND AX,00FFH
    MOV SI,AX
    MOV AL,KEY[SI]
    MOV DX,C8255PORTA
    OUT DX,AL       ;传送第二个数码管数据
    MOV AL,20H
    MOV DX,C8255PORTB
    OUT DX,AL       ;传送第二个数码管位控信号，点亮
    
    CALL    DELAY1
    
    MOV AL,00H
    MOV DX,C8255PORTB
    OUT DX,AL
    MOV AL,BUFFER[2]
    AND AX,00FFH
    MOV SI,AX
    MOV AL,KEY[SI]
    MOV DX,C8255PORTA
    OUT DX,AL       ;传送第三个数码管数据
    MOV AL,40H
    MOV DX,C8255PORTB
    OUT DX,AL       ;传送第三个数码管位控信号，点亮
    
    CALL    DELAY1
    
    MOV AL,00H
    MOV DX,C8255PORTB
    MOV DX,AL
    MOV AL,BUFFER[3]
    AND AX,00FFH
    MOV SI,AX
    MOV AL,KEY[SI]
    MOV DX,C8255PORTA
    OUT DX,AL   ;传送第四个数码管数据
    MOV AL,80H
    MOV DX,C8255PORTB
    OUT DX,AL   ;传送第四个数码管位控信号，点亮
    
    POP AX
    POP CX
    RET    
DISPLAY ENDP    
;------------延迟------------
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