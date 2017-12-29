# -*- coding: utf-8 -*                                      #语料为中文，采取UTF-8解编码
import re
import math
import sys



def getDictOfA(dict,filename):                              #获取用于计算状态转移矩阵A的字典
    f=open(filename,'r')
    for line in f.readlines():
        words=re.split(r'\s+',line)
        words.remove("")
        for i in range(0,len(words)-1):
            first=re.split('/',words[i])
            second=re.split('/',words[i+1])
            if len(first)>1 and len(second)>1:
                if first[1] in dict:
                    if second[1] in dict[first[1]]:         #统计词性转移频次
                        dict[first[1]][second[1]]+=1
                    else:
                        dict[first[1]][second[1]]=1
                else:
                    dict[first[1]]={'all':0}
                    dict[first[1]][second[1]]=1
                dict[first[1]]['all']+=1                    #统计总频次，为计算概率和平滑做准备
    f.close()
    return dict



def getDictOfB(dict,filename):                              #获取用于计算发射概率矩阵B的字典
    f=open(filename,'r')
    for line in f.readlines():
        words=re.split(r'\s+',line)
        words.remove("")
        for i in range(0,len(words)-1):
            tmp=re.split('/',words[i])
            if len(tmp)>1:
                if tmp[1] in dict:
                    if tmp[0] in dict[tmp[1]]:              #统计频次
                        dict[tmp[1]][tmp[0]]+=1
                    else:
                        dict[tmp[1]][tmp[0]]=1
                else:
                    dict[tmp[1]]={'all':0}
                    dict[tmp[1]][tmp[0]]=1
                dict[tmp[1]]['all']+=1                      #统计各词性的总频次，为计算概率和平滑做准备
    f.close()
    return dict



def getDictOfPi(dict,filename):                             #获取用于计算初始状态概率向量π的字典
    f=open(filename,'r')
    for line in f.readlines():
        words=re.split(r'\s+',line)
        tmp=re.split('/',words[0])
        if len(tmp)>1:
            if tmp[1] in dict:                              #统计各词性作为初始状态的频次
                dict[tmp[1]]+=1
            else:
                dict[tmp[1]]=1
    f.close()
    return dict



def outputA(f1,f2,f3,f4):                                   #输出状态转移矩阵A的字典
    dict={}
    dict=getDictOfA(dict,f1)
    dict=getDictOfA(dict,f2)
    dict=getDictOfA(dict,f3)
    dict=getDictOfA(dict,f4)
    f=open("A.txt",'w')
    items1=sorted(dict.items())
    for key1,value1 in items1:
        items2=sorted(value1.items())
        for key2,value2 in items2:
            f.write(key2+'/'+key1+' '+str(value2)+'\n')
    f.close()



def outputB(f1,f2,f3,f4):                                   #输出发射概率矩阵B的字典
    dict={}
    dict=getDictOfB(dict,f1)
    dict=getDictOfB(dict,f2)
    dict=getDictOfB(dict,f3)
    dict=getDictOfB(dict,f4)
    f=open("B.txt",'w')
    items1=sorted(dict.items())
    for key1,value1 in items1:
        items2=sorted(value1.items())
        for key2,value2 in items2:
            f.write(key1+'/'+key2+' '+str(value2)+'\n')
    f.close()



def outputPi(f1,f2,f3,f4):                                  #输出初始状态概率向量π的字典
    dict={}
    sum=0
    dict=getDictOfPi(dict,f1)
    dict=getDictOfPi(dict,f2)
    dict=getDictOfPi(dict,f3)
    dict=getDictOfPi(dict,f4)
    f=open("Pi.txt",'w')
    items=sorted(dict.items())
    for key,value in items:
        f.write(key+' '+str(value)+'\n')
        sum+=value
    f.write('all'+' '+str(sum)+'\n')
    f.close()



def getTransferMatrix():                                    #计算状态转移矩阵
    dict={}
    f=open("A.txt",'r')                                     #打开状态转移矩阵A的字典文件
    for line in f.readlines():
        tmp=re.split(r'\s+',line)
        words=re.split('/',tmp[0])
        if words[1] in dict:                                #记录各词性的总频次，以便计算概率
            if words[0] == 'all':
                dict[words[1]][words[0]]=tmp[1]
        else:
            if words[0] == 'all':
                dict[words[1]]={}
                dict[words[1]][words[0]]=tmp[1]
    f.seek(0)                                               #指针回至文件头
    for line in f.readlines():
        tmp=re.split(r'\s+',line)
        words=re.split('/',tmp[0])
        if words[1] in dict:                                #分别计算转移概率
            if words[0]!='all':
                dict[words[1]][words[0]]=float(tmp[1])/float(dict[words[1]]['all'])
    f.close()                                               #关闭字典文件
    return dict



def getLaunchMatrix():                                      #计算发射概率矩阵
    dict={}
    f=open("B.txt",'r')                                     #打开发射概率矩阵B的字典文件
    for line in f.readlines():
        tmp1=re.split(r'\s+',line)
        tmp2=re.split('/',tmp1[0])
        if tmp2[0] in dict:                                 #记录各词性的总频次，以便计算概率
            if tmp2[1]=='all':
                dict[tmp2[0]][tmp2[1]]=tmp1[1]
        else:
            if tmp2[1]=='all':
                dict[tmp2[0]]={}
                dict[tmp2[0]][tmp2[1]]=tmp1[1]
    f.seek(0)                                               #指针回至文件头
    for line in f.readlines():
        tmp1=re.split(r'\s+',line)
        tmp2=re.split('/',tmp1[0])
        if tmp2[1]!='all':
            if tmp2[0] in dict:                             #分别计算发射概率
                dict[tmp2[0]][tmp2[1]]=float(tmp1[1])/float(dict[tmp2[0]]['all'])
    f.close()                                               #关闭字典文件
    return dict



def getPOS():                                               #获取所有词性
    pos=[]
    f=open("A.txt",'r')
    for line in f.readlines():
        if 'all' in line:
            tmp1=re.split(r'\s+',line)
            tmp2=re.split('/',tmp1[0])
            if tmp2[1] not in pos:
                pos.append(tmp2[1])
    f.close()
    return pos



def getPi():                                                #计算初始状态概率向量
    dict={}
    f=open("Pi.txt",'r')                                    #打开初始状态概率向量π的字典文件
    for line in f.readlines():
        tmp=re.split(r'\s+',line)
        dict[tmp[0]]=tmp[1]
    sum=dict['all']
    for key in dict:                                        #分别计算概率
        dict[key]=float(dict[key])/float(sum)
    dict['all']*=float(sum)
    f.close()                                               #关闭字典文件
    return dict



def Viterbi(words,pos,pi,transM,launchM):                   #Viterbi算法解码
    t={}
    for item in pos:                                        #遍历所有词性
        if words[0] in launchM[item]:                       #若首词出现在发射概率矩阵中
            if item in pi:
                t[item]=(-math.log(pi[item]*launchM[item][words[0]]),[item])
            else:
                pi[item]=1.0/pi['all']                      #平滑处理
                t[item]=(-math.log(pi[item]*launchM[item][words[0]]),[item])
        else:                                               #否则，平滑处理
            if item in pi:
                t[item]=(-math.log(pi[item]*1.0/float(launchM[item]['all'])),[item])
            else:
                pi[item]=1.0/pi['all']
                t[item]=(-math.log(pi[item]*1.0/float(launchM[item]['all'])),[item])
    for i in range(1,len(words)):
        tmp={}
        for nextItem in pos:
            pMax=999999999999
            result=None
            for sourceItem in pos:
                (probability,path)=t[sourceItem]
                if sourceItem in transM:
                    if nextItem in transM[sourceItem]:
                        p1=transM[sourceItem][nextItem]
                    else:                                   #平滑处理
                        p1=0.0000000001
                else:                                       #平滑处理
                    p1=0.000000001
                if nextItem in launchM:
                    if words[i] in launchM[nextItem]:
                        p2=launchM[nextItem][words[i]]
                    else:                                   #平滑处理
                        p2=0.000000001
                p=p1*p2
                probability+=-math.log(p)
                if probability<pMax:
                    result=path+[nextItem]
                    pMax=probability
            tmp[nextItem]=(probability,result)
        t=tmp
    result=None
    pMax=999999999999
    for item in pos:
        (probability,path)=t[item]
        if probability<pMax:                                #寻找最优路径
            result=path
            pMax=probability
    return probability,result



def tagging(test,result):                                   #词性标注
    pi=getPi()
    transM=getTransferMatrix()
    launchM=getLaunchMatrix()
    pos=getPOS()
    resultfile=open(result,'w')
    testfile=open(test,'r')                                 #打开测试集
    for line in testfile:
        words=re.split(r'\s+',line)
        words.remove("")
        p,path=Viterbi(words,pos,pi,transM,launchM)
        for i in range(0,len(words)):
            if words[i] == '，':                             #利用基于规则的标注方法，提高正确率
                path[i] = 'wd'
            elif words[i] == '：':
                path[i] = 'wm'
            elif words[i] == '；':
                path[i] = 'wf'
            elif words[i] == '、':
                path[i] = 'wu'
            elif words[i] == '《':
                path[i] = 'wkz'
            elif words[i] == '》':
                path[i] = 'wky'
            elif words[i] == '：':
                path[i] = 'wm'
            elif '／' in words[i]:
                path[i] = 'm'
            elif words[i] == '“':
                path[i] = 'wyz'
            elif words[i] == '”':
                path[i] = 'wyy'
            elif words[i] == '（':
                path[i] = 'wkz'
            elif words[i] == '）':
                path[i] = 'wky'
            elif words[i] == '。':
                path[i] = 'wj'
            elif words[i] == '！':
                path[i] = 'wt'
            elif words[i] == '？':
                path[i] = 'ww'
            resultfile.write(words[i]+'/'+path[i]+' ')
        resultfile.write("\n")
    testfile.close()                                        #关闭测试集
    resultfile.close()




def calculateAccuracy(filename,resultFileName):             #计算标注正确率
    f=open(filename,'r')
    result=open(resultFileName,'r')
    tmp1=f.readlines()
    lines=result.readlines()
    right=0.0
    sum=0.0
    for i in range(0,len(tmp1)):
        tmp2=re.split(r'\s+',tmp1[i])
        words=re.split(r'\s+',lines[i])
        for j in range(0,len(tmp2)):
            tmp3=re.split('/',tmp2[j])
            word=re.split('/',words[j])
            if len(tmp3)>1 and len(word)>1:
                if tmp3[1]==word[1]:                        #比较标注结果与实际词性
                    right+=1
                sum+=1
    print("The correct number of words is "+str(right)+".")
    print("The total number of words is "+str(sum)+".")
    print("The accuracy is %.2f"%(100*right/sum)+"%.")



reload(sys)
sys.setdefaultencoding('UTF-8')
#五层交叉检验，评价模型性能
print("***********************************************************************************************")
print(">>                           The first part has been POS labeled.                           <<")
outputA("second corpus.txt","third corpus.txt","fourth corpus.txt","fifth corpus.txt")
outputB("second corpus.txt","third corpus.txt","fourth corpus.txt","fifth corpus.txt")
outputPi("second corpus.txt","third corpus.txt","fourth corpus.txt","fifth corpus.txt")
tagging("first test.txt","first result.txt")
calculateAccuracy("first result.txt","first corpus.txt")
print("***********************************************************************************************")
print(">>                          The second part has been POS labeled.                           <<")
outputA("first corpus.txt","third corpus.txt","fourth corpus.txt","fifth corpus.txt")
outputB("first corpus.txt","third corpus.txt","fourth corpus.txt","fifth corpus.txt")
outputPi("first corpus.txt","third corpus.txt","fourth corpus.txt","fifth corpus.txt")
tagging("second test.txt","second result.txt")
calculateAccuracy("second result.txt","second corpus.txt")
print("***********************************************************************************************")
print(">>                           The third part has been POS labeled.                           <<")
outputA("first corpus.txt","second corpus.txt","fourth corpus.txt","fifth corpus.txt")
outputB("first corpus.txt","second corpus.txt","fourth corpus.txt","fifth corpus.txt")
outputPi("first corpus.txt","second corpus.txt","fourth corpus.txt","fifth corpus.txt")
tagging("third test.txt","third result.txt")
calculateAccuracy("third result.txt","third corpus.txt")
print("***********************************************************************************************")
print(">>                          The fourth part has been POS labeled.                           <<")
outputA("first corpus.txt","second corpus.txt","third corpus.txt","fifth corpus.txt")
outputB("first corpus.txt","second corpus.txt","third corpus.txt","fifth corpus.txt")
outputPi("first corpus.txt","second corpus.txt","third corpus.txt","fifth corpus.txt")
tagging("fourth test.txt","fourth result.txt")
calculateAccuracy("fourth result.txt","fourth corpus.txt")
print("***********************************************************************************************")
print(">>                           The fifth part has been POS labeled.                           <<")
outputA("first corpus.txt","second corpus.txt","third corpus.txt","fourth corpus.txt")
outputB("first corpus.txt","second corpus.txt","third corpus.txt","fourth corpus.txt")
outputPi("first corpus.txt","second corpus.txt","third corpus.txt","fourth corpus.txt")
tagging("fifth test.txt","fifth result.txt")
calculateAccuracy("fifth result.txt","fifth corpus.txt")
print("***********************************************************************************************")