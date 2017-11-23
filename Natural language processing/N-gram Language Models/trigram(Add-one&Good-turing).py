# -*- coding: utf-8 -*                                          #语料为中文，采用UTF-8编解码
import re
import math


#该类用于实现Trigram
class trigram:
    def __init__(self):                                         #初始化
        self.uniDict={}
        self.biDict={}
        self.triDict={}
        self.V=0
        self.N=0
        self.uniN=0
        self.biN=0
        file=open("uniDict.txt",'r')                            #打开unigram词典文件
        for line in file.readlines():
            word=re.split(r'\s+',line)
            word.remove("")
            self.uniDict[word[0]]=word[1]
            self.uniN+=int(word[1])
        self.uniDict["<unknown>"]=0                             #预留项
        file.close()                                            #关闭unigram词典文件

        file=open("biDict.txt",'r')                             #打开bigram词典文件
        for line in file.readlines():
            words=re.split(r'\s+',line)
            words.remove("")
            word=re.split('/',words[0])
            if word[1] in self.biDict:
                self.biDict[word[1]][word[0]]=words[1]
            else:
                self.biDict[word[1]]={}
                self.biDict[word[1]]["<unknown>"]=0             #预留项
                self.biDict[word[1]][word[0]]=words[1]
            self.biN+=int(words[1])
        file.close()                                            #关闭bigram词典文件

        file=open("triDict.txt",'r')                            #打开trigram词典文件
        for line in file.readlines():
            words=re.split(r'\s+',line)
            words.remove("")
            word=re.split('/',words[0])
            if word[1]+'/'+word[2] in self.triDict:
                self.triDict[word[1]+'/'+word[2]][word[0]]=words[1]
            else:
                self.triDict[word[1]+'/'+word[2]]={}
                self.triDict[word[1]+'/'+word[2]]["<unknown>"]=0
                self.V+=1
                self.triDict[word[1]+'/'+word[2]][word[0]]=words[1]
            self.V+=1
            self.N+=int(words[1])
        file.close()                                            #关闭trigram词典文件


    def __del__(self):
        pass


    def getPerplexity(self,p,n):                                #计算困惑度
        return math.pow(2,-p/n)


    def add_one(self):                                          #Add-one平滑方法
        uni=self.uniDict.copy()
        bi=self.biDict.copy()
        smoothDict=self.triDict.copy()
        for key1 in smoothDict.keys():                          #对trigram词典进行加一处理
            for key2 in smoothDict[key1].keys():
                words=re.split('/',key1)
                smoothDict[key1][key2]=math.log((int(smoothDict[key1][key2])+1.0)/(len(smoothDict[key1])+len(bi[words[1]][words[0]])),2)

        for key1 in bi.keys():                                  #对bigram词典进行加一处理
            for key2 in bi[key1].keys():
                if key1!="start":
                    bi[key1][key2]=math.log((int(bi[key1][key2])+1.0)/(len(bi[key1])+int(uni[key1])),2)
                else:
                    bi[key1][key2]=math.log((int(bi[key1][key2])+1.0)/len(bi[key1]),2)

        for key in uni.keys():                                  #对unigram词典进行加一处理
            uni[key]=math.log((int(uni[key])+1.0)/(len(uni)+self.uniN),2)

        p=0.0
        testN=0
        file=open("test.txt",'r')                               #打开测试文件
        for line in file.readlines():                           #计算测试语料出现概率
            words=re.split(r'\s+',line)
            words.remove("")
            testN+=len(words)
            words.insert(0,"start")
            words.append("$")
            for i in range(0,len(words)-2):
                if words[i+1]+'/'+words[i] in smoothDict:
                    if words[i+2] in smoothDict[words[i+1]+'/'+words[i]]:
                        p+=smoothDict[words[i+1]+'/'+words[i]][words[i+2]]
                    else:
                        p+=smoothDict[words[i+1]+'/'+words[i]]["<unknown>"]
                else:
                    if words[i] in bi:
                        p+=bi[words[i]]["<unknown>"]
                    else:
                        p+=uni["<unknown>"]
                    if words[i+2]=="$":
                        if words[i+1] in bi:
                            if words[i+2] in bi[words[i+1]].keys():
                                p+=bi[words[i+1]][words[i+2]]
                            else:
                                p+=bi[words[i+1]]["<unknown>"]
                        else:
                            p+=uni["<unknown>"]

        perplexity=self.getPerplexity(p,testN)                  #计算困惑度
        print(">>                       The perplexity of Add-one smoothing is %.2f.                       <<"%perplexity)
        file.close()                                            #关闭测试文件


    def good_turing(self):                                      #Good-turing平滑方法
        uni=self.uniDict.copy()
        bi=self.biDict.copy()
        smoothDict=self.triDict.copy()
        for key1 in bi.keys():                                  #对bigram词典进行加一处理
            for key2 in bi[key1]:
                if key1!="start":
                    bi[key1][key2]=math.log((int(bi[key1][key2])+1.0)/(len(bi[key1])+int(uni[key1])),2)
                else:
                    bi[key1][key2]=math.log((int(bi[key1][key2])+1.0)/len(bi[key1]),2)

        for key in uni.keys():                                  #对unigram词典进行加一处理
            uni[key]=math.log((int(uni[key])+1.0)/(len(uni)+self.uniN),2)

        for key1 in smoothDict.keys():                          #对trigram词典进行分频率计算
            Nc={}
            for key2 in smoothDict[key1].keys():
                if smoothDict[key1][key2] in Nc:
                    Nc[smoothDict[key1][key2]]+=1.0
                else:
                    Nc[smoothDict[key1][key2]]=1.0
            tmp=sorted(Nc.items(),key=lambda item:int(item[0]),reverse=False)
            for i in range(0,len(tmp)-1):                       #计算折扣
                tmp[i]=(tmp[i][0],float(tmp[i+1][0])*tmp[i+1][1]/tmp[i][1])
            N=0.0
            for i in range(0,len(tmp)):
                N+=tmp[i][1]
            for i in range(0,len(tmp)):
                tmp[i]=(tmp[i][0],tmp[i][1]/N)
                Nc[tmp[i][0]]=tmp[i][1]

        p=0.0
        testN=0
        file=open("test.txt",'r')                               #打开测试文件
        for line in file.readlines():
            words=re.split(r'\s+',line)
            words.remove("")
            testN+=len(words)
            words.insert(0,"start")
            words.append("$")
            for i in range(0,len(words)-2):                     #计算测试语料出现概率
                if words[i+1]+'/'+words[i] in smoothDict:
                    if words[i+2] in smoothDict[words[i+1]+'/'+words[i]]:
                        p+=float(smoothDict[words[i+1]+'/'+words[i]][words[i+2]])
                    else:
                        p+=float(smoothDict[words[i+1]+'/'+words[i]]["<unknown>"])
                else:
                    if words[i] in bi:
                        p+=bi[words[i]]["<unknown>"]
                    else:
                        p+=uni["<unknown>"]
                    if words[i+2]=="$":
                        if words[i+1] in bi:
                            if words[i+2] in bi[words[i+1]].keys():
                                p+=bi[words[i+1]][words[i+2]]
                            else:
                                p+=bi[words[i+1]]["<unknown>"]
                        else:
                            p+=uni["<unknown>"]

        perplexity=self.getPerplexity(p,testN)                  #计算困惑度
        print(">>                      The perplexity of Good-turing smoothing is %.2f.                     <<"%perplexity)
        file.close()                                            #关闭测试文件



print(">>                                          trigram                                          <<")
print("***********************************************************************************************")
gram1=trigram()                                                 #创建对象
gram1.add_one()                                                 #Add-one平滑
gram2=trigram()
print("***********************************************************************************************")
gram2.good_turing()                                             #Good-turing平滑
print("***********************************************************************************************")