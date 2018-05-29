.text
main:
ADDIU $r1,$r0,array1                #取向量一首地址
ADDIU $r2,$r0,array2                #取向量二首地址
ADDI  $r3,$r0,10                    #向量维度
ADDI  $r7,$r0,0                     #清零结果寄存器
loop:  
LW    $r4,0($r1)                    #取相应元素
LW    $r5,0($r2)
MUL   $r6,$r4,$r5
ADDI  $r1,$r1,4                     #计算下一元素地址
ADDI  $r2,$r2,4
ADDI  $r3,$r3,-1
ADD   $r7,$r6,$r7                   #优化
BGTZ  $r3,loop                      
TEQ   $r0,$r0

.data
array1:.word 0,1,2,3,4,5,6,7,8,9
array2:.word 0,1,2,3,4,5,6,7,8,9