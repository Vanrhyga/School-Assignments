# -*- coding: utf-8 -*                                           #语料为中文，采用UTF-8编解码
import re
import math


#该类用于实现Bigram
class bigram:
    def __init__(self):                                         #初始化
        self.uniDict={}
        self.biDict={}
        self.N=0
        self.V=0
        self.uniN=0
        file=open("uniDict.txt",'r')                            #打开unigram词典文件
        for line in file.readlines():
            word=re.split(r'\s+',line)
            word.remove("")
            self.uniDict[word[0]]=word[1]
            self.uniN+=int(word[1])
        self.uniDict["<unknown>"]=0
        file.close()                                            #关闭unigram词典文件

        file=open("biDict.txt",'r')                             #打开bigram词典文件
        for line in file.readlines():                           #构造bigram字典
            words=re.split(r'\s+',line)
            words.remove("")
            word=re.split('/',words[0])
            if word[1] in self.biDict:
                self.biDict[word[1]][word[0]]=words[1]
            else:
                self.biDict[word[1]]={}
                self.biDict[word[1]]["<unknown>"]=0             #预留项
                self.V+=1                                       #计算bigram词表大小
                self.biDict[word[1]][word[0]]=words[1]
            self.V+=1
            self.N+=int(words[1])
        file.close()                                           #关闭unigram词典文件


    def __del__(self):
        pass


    def getPerplexity(self,p,n):                                #计算困惑度
        return math.pow(2,-p/n)


    def add_one(self):                                          #Add-one平滑方法
        uni=self.uniDict.copy()
        smoothDict=self.biDict.copy()
        for key1 in smoothDict.keys():                          #对bigram词典进行加一处理
            for key2 in smoothDict[key1].keys():
                if key1!="start":
                    smoothDict[key1][key2]=math.log((int(smoothDict[key1][key2])+1.0)/(len(smoothDict[key1])+int(uni[key1])),2)
                else:
                    smoothDict[key1][key2]=math.log((int(smoothDict[key1][key2])+1.0)/len(smoothDict[key1]),2)

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
            for i in range(0,len(words)-1):
                if words[i] in smoothDict:
                    if words[i+1] in smoothDict[words[i]]:
                        p+=float(smoothDict[words[i]][words[i+1]])
                    else:
                        p+=float(smoothDict[words[i]]["<unknown>"])
                else:
                    p+=uni["<unknown>"]

        perplexity=self.getPerplexity(p,testN)                  #计算困惑度
        print(">>                       The perplexity of Add-one smoothing is %.2f.                      <<"%perplexity)
        file.close()                                            #关闭测试文件


    def good_turing(self):                                      #Good-turing平滑方法
        uni=self.uniDict.copy()
        smoothDict=self.biDict.copy()
        for key in uni.keys():                                  #对unigram词典进行加一处理
            uni[key]=math.log((int(uni[key])+1.0)/(len(uni)+self.uniN),2)

        for key1 in smoothDict.keys():                          #对bigram词典进行分频率计算
            Nc={}
            for key2 in smoothDict[key1].keys():
                if smoothDict[key1][key2] in Nc:
                    Nc[smoothDict[key1][key2]]+=1.0
                else:
                    Nc[smoothDict[key1][key2]]=1.0
            tmp=sorted(Nc.items(),key=lambda item:int(item[0]),reverse=False)
            for i in range(0,len(tmp)-1):                       #计算折扣
                tmp[i]=(tmp[i][0],int(tmp[i+1][0])*tmp[i+1][1]/tmp[i][1])
            N=0.0
            for i in range(0,len(tmp)):
                N+=tmp[i][1]
            for i in range(0,len(tmp)):                         #计算单词出现概率
                tmp[i]=(tmp[i][0],tmp[i][1]/N)
                Nc[tmp[i][0]]=tmp[i][1]
            for key2 in smoothDict[key1].keys():
                smoothDict[key1][key2]=math.log(Nc[smoothDict[key1][key2]],2)

        p=0.0
        testN=0
        file=open("test.txt",'r')                               #打开测试文件
        for line in file.readlines():
            words=re.split(r'\s+',line)
            words.remove("")
            testN+=len(words)
            words.insert(0,"start")
            words.append("$")
            for i in range(0,len(words)-1):                     #计算测试语料出现概率
                if words[i] in smoothDict:
                    if words[i+1] in smoothDict[words[i]]:
                        p+=smoothDict[words[i]][words[i+1]]
                    else:
                        p+=smoothDict[words[i]]["<unknown>"]
                else:
                    p+=uni["<unknown>"]

        perplexity=self.getPerplexity(p,testN)                  #计算困惑度
        print(">>                      The perplexity of Good-turing smoothing is %.2f.                    <<"%perplexity)
        file.close()                                            #关闭测试文件



print(">>                                          bigram                                           <<")
print("***********************************************************************************************")
gram1=bigram()                                                   #创建对象
gram1.add_one()                                                  #Add-one平滑

gram2=bigram()
print("***********************************************************************************************")
gram2.good_turing()                                              #Good-turing平滑
print("***********************************************************************************************")