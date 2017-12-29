# -*- coding: utf-8 -*                                              #语料为中文，采取UTF-8编解码
import codecs
import re



with codecs.open("train.txt",'r',encoding='UTF-8') as train:        #打开训练集
    dictForTraining={}
    maxWordLength=0
    for line in train.readlines():
        words=re.split(r'\s+',line)
        for i in range(0,len(words)):
            if words[i] in dictForTraining:                         #生成词典
                dictForTraining[words[i]]+=1
            else:
                dictForTraining[words[i]]=1
            if len(words[i])>maxWordLength:                         #记录最大词条长度
                maxWordLength=len(words[i])
    for key in dictForTraining:
        if len(key)>1 and key[0]=='[':
            key=key[1:]
train.close()                                                       #关闭训练集



with codecs.open("test.txt",'r',encoding='UTF-8') as test:          #打开测试文本
    with codecs.open("result.txt",'w',encoding='UTF-8') as result:
        record=[]
        segmentResult=[]
        for line in test.readlines():
            i=0
            outDict=False
            inDict=False
            record[:]=[]
            while i<len(line):
                for j in range(maxWordLength,0,-1):                 #按递减方式截取子串
                    if i+j<=len(line):
                        if line[i:i+j] in dictForTraining:          #若子串在词典中
                            if outDict:                             #若前一个词是未出现词
                                record.append(i)                    #记录未出现词截止下标
                                outDict=False
                            record.append(i+j)                      #记录当前子串截止下标
                            i+=j                                    #更新匹配起点
                            inDict=True
                            break
                if inDict is False:                                 #若所有子串均不在词典中
                    outDict=True                                    #存在未出现词
                    i+=1                                            #匹配起点后移一位
                else:
                    inDict=False
            if outDict:                                             #若最后一个词是未出现词
                record.append(i+1)                                  #记录截止下标
            if len(record)>0:                                       #生成分词结果
                segmentResult.append(line[:record[0]])
            for i in range(0,len(record)-1):
                segmentResult.append(line[record[i]:record[i+1]])
            for i in range(0,len(segmentResult)-1):
                result.write(segmentResult[i]+" ")
            result.write(segmentResult[len(segmentResult)-1])
            segmentResult[:]=[]
    result.close()
test.close()                                                        #关闭测试文本
print("***********************************************************************************************")
print(">>                           The word segmentation has been over.                            <<")
print("***********************************************************************************************")



with codecs.open("answer.txt",'r',encoding='UTF-8') as answer:      #打开分词答案
    with codecs.open("result.txt",'r',encoding='UTF-8') as result:  #打开分词结果文件
        correct=0
        total=0
        reference=0
        for line1 in answer.readlines():
            line2=result.readline()
            words1=re.split(r'\s+',line1)
            words2=re.split(r'\s+',line2)
            reference+=len(words1)                                  #统计答案中分词总个数
            total+=len(words2)                                      #统计分词总个数
            for word in words1:
                if word in words2:                                  #统计正确分词个数
                    correct+=1
        precision=float(correct)/total                              #计算正确率
        recall=float(correct)/reference                             #计算召回率
        F=2*precision*recall/(precision+recall)                     #计算F值
    result.close()                                                  #关闭分词结果文件
answer.close()                                                      #关闭分词答案
print("The number of the words in the reference is "+str(reference))
print("The number of the words segmented is "+str(total))
print("The number of the words correctly segmented is "+str(correct))
print("Precision:%.2f"%(precision*100)+"%")
print("Recall:%.2f"%(recall*100)+"%")
print("The accuracy is %.2f"%(F*100)+"%.")
print("***********************************************************************************************")