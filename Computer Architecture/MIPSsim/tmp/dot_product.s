.text
main:
ADDIU $r1,$r0,array1                #ȡ����һ�׵�ַ
ADDIU $r2,$r0,array2                #ȡ�������׵�ַ
ADDI  $r3,$r0,10                    #����ά��
ADDI  $r7,$r0,0                     #�������Ĵ���
loop:  
LW    $r4,0($r1)                    #ȡ��ӦԪ��
LW    $r5,0($r2)
MUL   $r6,$r4,$r5
ADDI  $r1,$r1,4                     #������һԪ�ص�ַ
ADDI  $r2,$r2,4
ADDI  $r3,$r3,-1
ADD   $r7,$r6,$r7                   #�Ż�
BGTZ  $r3,loop                      
TEQ   $r0,$r0

.data
array1:.word 0,1,2,3,4,5,6,7,8,9
array2:.word 0,1,2,3,4,5,6,7,8,9