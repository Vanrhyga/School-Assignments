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
    ASCII   DB  46H,45H,44H,43H,42H,41H,39H,38H,37H,36H,35H,34H,33H,32H,31H,30H ;ASCII码
    BUFFER  DB  00H,00H,00H,00H
    LINEKEY DB  ? 
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
    
    MOV AL,01101111B;方式命令字，异步方式，2bit停止位，无校验，8bit数据位，波特率因子64
    MOV DX,C8251PORTCTRL
    OUT DX,AL
    
    MOV AL,00100111B;工作命令字，正常操作，请求发送，复位错误标志，接受发送允许
    OUT DX,AL
    
;初始化8254
    MOV AL,00010110B;控制字，计数器0，低字节，方式3，二进制计数
    MOV DX,C8254PORTCTRL
    OUT DX,AL
    
    MOV AX,24H      ;计数初值
    MOV DX,C8254PORT0
    OUT DX,AL       ;写入计数初值
    
;初始化8255
    MOV AL,10000001B;选择控制字，A组方式0，A口输出，C口高四位输出，B组方式0，B口输出，C组第四位输入
    MOV DX,C8255PORTCTRL
    OUT DX,AL
    
;循环扫描键盘及8251状态口
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
    IN  AL,DX   ;读入列值   
    AND AL,0FH  ;读低4位
    CMP AL,0FH
    
    JZ  SCAN2   ;无键按下          
              	
    CALL	DELAY2    

    MOV BL,4    ;行数
    MOV BH,4    ;列数
    MOV AL,0EFH ;扫描码  
    MOV CH,0FFH     
    
ROW:
    MOV DX,C8255PORTC
    OUT DX,AL   ;C口高四位输出扫描码     
    ROL AL,1    ;循环左移生成下一扫描码
    MOV AH,AL   ;保存下一扫描码
    MOV DX,C8255PORTC
    IN  AL,DX   ;C口四位读入列值
    MOV LINEKEY,AL           
    AND AL,0FH ;低四位
    CMP AL,0FH  ;判断是否有列线为0
    JNZ HANDLE     
    ADD CH,BH
    MOV AL,AH   ;取扫描码
    DEC BL      ;行计数-1
    JNZ ROW     ;行数不为0，继续扫描
    
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
    JC  COL ;最低位为1，未按下，继续循环
    
    MOV AL,CH   ;保存键下标值

    CALL MOVE

;8251发送
    CMP AL,01H  ;按下E时退出
    JZ  EXIT

    PUSH AX

    MOV DX,C8251PORTCTRL
    IN AL,DX
    AND AL,01H  
    POP AX
    JZ SCAN2

    MOV DX,C8251PORTIO
    OUT DX,AL   ;发送键值 

SCAN2:
    MOV DX,C8251PORTCTRL
    IN  AL,DX   ;读状态口
    AND AL,02H  ;判断RxRDY=1?

    JZ  INPUT

    MOV DX,C8251PORTIO
    IN  AL,DX   ;接收数据

    CALL MOVE

    JMP INPUT
EXIT:
    MOV AX,4C00H
    INT 21H ;返回DOS     
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
    MOV AL,80H
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
    MOV AL,40H
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
    MOV AL,20H
    MOV DX,C8255PORTB
    OUT DX,AL       ;传送第三个数码管位控信号，点亮
    
    CALL    DELAY1
    
    MOV AL,00H
    MOV DX,C8255PORTB
    OUT DX,AL
    MOV AL,BUFFER[3]
    AND AX,00FFH
    MOV SI,AX
    MOV AL,KEY[SI]
    MOV DX,C8255PORTA
    OUT DX,AL   ;传送第四个数码管数据
    MOV AL,10H

    MOV DX,C8255PORTB
    OUT DX,AL   ;传送第四个数码管位控信号，点亮

    CALL DELAY1
    MOV AL,00H
    MOV DX,C8255PORTB
    OUT DX,AL
    
    POP AX
    POP CX
    RET    
DISPLAY ENDP   
;------------移位------------
MOVE	PROC
    PUSH    AX
    MOV AL,BUFFER[2]
    MOV BUFFER[3],AL
    MOV AL,BUFFER[1]
    MOV BUFFER[2],AL
    MOV AL,BUFFER[0]
    MOV BUFFER[1],AL
    POP AX
    MOV BUFFER[0],AL    ;新数据放入BUFFER[0]中
MOVE	ENDP
;------------屏幕显示------------
DISPLAYONSCREEN PROC
    PUSH AX
    AND AX,0FH
    MOV SI,AX
    MOV DL,ASCII[SI]  ;按键对应ASCII码   
    MOV AH,02H
    INT 21H
    POP AX
DISPLAYONSCREEN ENDP
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