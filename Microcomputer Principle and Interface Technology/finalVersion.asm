;常量
C8251PORTIO     EQU 2B8H    ;8251 IO端口
C8251PORTCTRL   EQU 2B9H    ;8251 控制端口
C8254PORT0      EQU 280H    ;8254 端口0
C8254PORTCTRL   EQU 283H    ;8254 控制端口
C8255PORTA      EQU 288H    ;8255 A口
C8255PORTB      EQU 289H    ;8255 B口
C8255PORTC      EQU 28AH    ;8255 C口
C8255PORTCTRL   EQU 28BH    ;8255 控制端口

;数据段
DATA    SEGMENT
    KEY     DB  71H,79H,5EH,39H,7CH,77H,6FH,7FH,07H,7DH,6DH,66H,4FH,5BH,06H,3FH ;数码管
    ASCII   DB  46H,45H,44H,43H,42H,41H,39H,38H,37H,36H,35H,34H,33H,32H,31H,30H ;ASCII码
    BUFFER1 DB  00H,00H,00H,00H 
    BUFFER2 DB  00H,00H,00H,00H    
    LCD_END DB  45H,4EH,44H ;END
    LINEKEY DB  ?
DATA    ENDS

;代码段
CODE    SEGMENT
    ASSUME CS:CODE,DS:DATA
    
START:
    MOV AX,DATA
    MOV DS,AX               ;DS指向数据段
    
;初始化8251
    MOV DX,C8251PORTCTRL
    XOR AL,AL
    OUT DX,AL               ;预初始化
    MOV AL,40H
    OUT DX,AL               ;软复位 
    
    NOP                     ;延时
    
    MOV AL,01001111B        ;方式命令字：异步方式，1bit停止位，无校验，8bit数据位，波特率因子64
    MOV DX,C8251PORTCTRL
    OUT DX,AL
    
    MOV AL,00100111B        ;工作命令字：正常操作，请求发送，接受发送允许
    OUT DX,AL
    
;初始化8254
    MOV AL,00010100B        ;控制字：计数器0，低字节，方式2，二进制计数
    MOV DX,C8254PORTCTRL
    OUT DX,AL
    
    MOV AX,24H              ;计数初值
    MOV DX,C8254PORT0
    OUT DX,AL      
    
;初始化8255
    MOV AL,10000001B        ;方式选择控制字：A组方式0，A口输出，C口高四位输出，B组方式0，B口输出，C组低四位输入
    MOV DX,C8255PORTCTRL
    OUT DX,AL

;初始化液晶屏
    CALL CLEAR              ;清屏
    
    CALL DISPLAYONLCD       ;显示
    
;循环扫描键盘及8251状态口
INPUT:
    MOV AL,00H
    MOV DX,C8255PORTC
    OUT DX,AL
   
SCAN1:
    MOV AL,00H
    MOV DX,C8255PORTB
    OUT DX,AL
    
    CALL DISPLAY            ;数码管显示
    CALL DISPLAYONLCD       ;LCD显示   
    
    MOV DX,C8255PORTC
    IN  AL,DX               ;读入列值   
    AND AL,0FH              ;取低4位
    CMP AL,0FH
    JZ  SCAN2               ;无键按下          
              	
    CALL DELAY2    

    MOV BL,4                ;行数
    MOV BH,4                ;列数
    MOV AL,0EFH             ;扫描码  
    MOV CH,0FFH             ;小键盘下标值     
    
ROW:
    MOV DX,C8255PORTC
    OUT DX,AL               ;C口高四位输出扫描码     
    ROL AL,1                ;循环左移生成下一扫描码
    MOV AH,AL               ;保存下一扫描码
    
    MOV DX,C8255PORTC
    IN  AL,DX               ;C口低四位读入列值
    MOV LINEKEY,AL           
    AND AL,0FH              ;取低四位
    CMP AL,0FH              ;判断是否有列线为0
    JNZ HANDLE              ;有键按下
    
    ADD CH,BH               ;下标增加
    MOV AL,AH               ;取扫描码
    DEC BL                  ;行计数-1
    JNZ ROW                 ;行数不为0，继续扫描
    
    JMP SCAN2
        
HANDLE:
    MOV DX,C8255PORTC
    CALL DELAY
    IN  AL,DX
    CMP AL,LINEKEY
    JZ  HANDLE              ;按键未抬起
    
    MOV AL,LINEKEY
    
COL:
    INC CH                  ;下标值增加
    RCR AL,1
    JC  COL                 ;未按下
    
    MOV AL,CH               ;保存下标值
   
    CALL DISPLAYONSCREEN    ;屏幕显示

    CALL MOVE1              ;数字左移

;8251发送
    CMP AL,01H  
    JZ  EXIT                ;按下E，退出

    PUSH AX
    MOV DX,C8251PORTCTRL
    IN  AL,DX
    AND AL,01H               
    POP AX
    JZ  SCAN2               ;发送器未准备好

    MOV DX,C8251PORTIO
    OUT DX,AL               ;发送键值 

SCAN2:
    MOV DX,C8251PORTCTRL
    IN  AL,DX               ;读状态口
    AND AL,02H  
    JZ  INPUT               ;接收器未准备好

    MOV DX,C8251PORTIO
    IN  AL,DX               ;接收数据

    CALL MOVE2              ;数字左移

    JMP INPUT   
    
EXIT:        
    CALL LCD_EXIT           ;LCD退出
    MOV AX,4C00H
    INT 21H                 ;返回DOS     
;------------数码管显示------------
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
    OUT DX,AL               ;送入第一个数码管数据
    MOV AL,80H
    MOV DX,C8255PORTB
    OUT DX,AL               ;送入第一个数码管位控信号
    
    CALL DELAY1
                  
    MOV AL,00H
    MOV DX,C8255PORTB
    OUT DX,AL
    MOV AL,BUFFER1[1]
    AND AX,00FFH
    MOV SI,AX
    MOV AL,KEY[SI]
    MOV DX,C8255PORTA
    OUT DX,AL               ;送入第二个数码管数据
    MOV AL,40H
    MOV DX,C8255PORTB
    OUT DX,AL               ;送入第二个数码管位控信号
    
    CALL DELAY1
    
    MOV AL,00H
    MOV DX,C8255PORTB
    OUT DX,AL
    MOV AL,BUFFER1[2]
    AND AX,00FFH
    MOV SI,AX
    MOV AL,KEY[SI]
    MOV DX,C8255PORTA
    OUT DX,AL               ;送入第三个数码管数据
    MOV AL,20H
    MOV DX,C8255PORTB
    OUT DX,AL               ;送入第三个数码管位控信号
    
    CALL DELAY1
    
    MOV AL,00H
    MOV DX,C8255PORTB
    OUT DX,AL
    MOV AL,BUFFER1[3]
    AND AX,00FFH
    MOV SI,AX
    MOV AL,KEY[SI]
    MOV DX,C8255PORTA
    OUT DX,AL               ;送入第四个数码管数据
    MOV AL,10H

    MOV DX,C8255PORTB
    OUT DX,AL               ;送入第四个数码管位控信号

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
;------------屏幕显示------------
DISPLAYONSCREEN PROC
    PUSH AX
    
    AND AX,0FH
    MOV SI,AX
    MOV DL,ASCII[SI]        ;按键对应ASCII码   
    MOV AH,02H
    INT 21H

    CALL DISPLAY            ;数码管显示

    POP AX
    RET
DISPLAYONSCREEN ENDP   
;------------液晶屏显示------------
DISPLAYONLCD PROC
    PUSH DX
    PUSH AX
    PUSH SI
    
;显示第一个数字
    MOV DX,C8255PORTA
    MOV AL,96H
    OUT DX,AL               ;设定显示位置
    
    CALL LCD_CTRL           ;送入指令
    
    MOV DX,C8255PORTA
    MOV AL,BUFFER2[0]
    AND AX,0FFH
    MOV SI,AX
    MOV AL,ASCII[SI]
    OUT DX,AL
    
    CALL LCD_DATA           ;送入数据    
    
    CALL DELAY
    
;显示第二个数字
    MOV DX,C8255PORTA
    MOV AL,94H  
    OUT DX,AL               ;设定显示位置
    
    CALL LCD_CTRL           ;送入指令
    
    MOV DX,C8255PORTA
    MOV AL,BUFFER2[1]
    AND AX,0FFH
    MOV SI,AX
    MOV AL,ASCII[SI]
    OUT DX,AL
    
    CALL LCD_DATA           ;送入数据
    
    CALL DELAY
    
;显示第三个数字
    MOV DX,C8255PORTA
    MOV AL,92H
    OUT DX,AL               ;设定显示位置
    
    CALL LCD_CTRL           ;送入指令
    
    MOV DX,C8255PORTA
    MOV AL,BUFFER2[2]
    AND AX,0FFH
    MOV SI,AX
    MOV AL,ASCII[SI]
    OUT DX,AL   
    
    CALL LCD_DATA           ;送入数据
    
    CALL DELAY
             
;显示第四个数字
    MOV DX,C8255PORTA
    MOV AL,90H
    OUT DX,AL               ;设定显示位置
    
    CALL LCD_CTRL           ;送入指令
    
    MOV DX,C8255PORTA
    MOV AL,BUFFER2[3]
    AND AX,0FFH
    MOV SI,AX
    MOV AL,ASCII[SI]
    OUT DX,AL   
    
    CALL LCD_DATA           ;送入数据
    
    CALL DELAY
    
    POP SI
    POP AX
    POP DX               
    RET               
DISPLAYONLCD    ENDP    
;------------液晶屏送入指令------------
LCD_CTRL    PROC
    MOV DX,C8255PORTB   
    MOV AL,00000000B        ;写指令
    OUT DX,AL
    
    CALL DELAY

;锁存    
    MOV AL,00000100B
    OUT DX,AL
    
    CALL DELAY
    
    MOV AL,00000000B
    OUT DX,AL
    
    CALL DELAY

    CALL DISPLAY            ;数码管显示
    
    RET   
LCD_CTRL    ENDP       
;------------液晶屏送入数据------------
LCD_DATA    PROC
    MOV DX,C8255PORTB
    
    MOV AL,00000001B        ;写数据
    OUT DX,AL
    
    CALL DELAY

;锁存    
    MOV AL,00000101B
    OUT DX,AL
    
    CALL DELAY
    
    MOV AL,00000001B
    OUT DX,AL
    
    CALL DELAY
    
    RET   
LCD_DATA    ENDP
;------------液晶屏清屏------------
CLEAR   PROC
    MOV DX,C8255PORTA
    MOV AL,00001100B    
    OUT DX,AL               ;启动显示
    
    CALL LCD_CTRL           ;送入指令
    
    MOV DX,C8255PORTA
    MOV AL,00000001B    
    OUT DX,AL               ;清屏
    
    CALL LCD_CTRL           ;送入指令
                      
    RET      
CLEAR   ENDP                 
;------------液晶屏退出------------
LCD_EXIT    PROC
    CALL CLEAR
    
    MOV SI,0
    MOV BL,92H              ;设定显示位置
    MOV CX,3
    
LP:
    MOV DX,C8255PORTA
    MOV AL,BL
    OUT DX,AL               ;设定显示位置
    
    CALL LCD_CTRL           ;送入指令
    
    MOV DX,C8255PORTA
    MOV AL,LCD_END[SI]
    OUT DX,AL
    
    CALL LCD_DATA           ;送入数据
    
    INC SI
    INC BL
    LOOP LP
    
    RET 
LCD_EXIT    ENDP
;------------移位------------
MOVE1	PROC
    PUSH AX
    
    MOV AL,BUFFER1[2]
    MOV BUFFER1[3],AL
    MOV AL,BUFFER1[1]
    MOV BUFFER1[2],AL
    MOV AL,BUFFER1[0]
    MOV BUFFER1[1],AL
    
    POP AX
    
    MOV BUFFER1[0],AL        ;新数据放入BUFFER1[0]中

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
    
    MOV BUFFER2[0],AL        ;新数据放入BUFFER2[0]中

    RET
MOVE2	ENDP
;------------延迟------------
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