# -*- coding: utf-8 -*                                                          #语料为中文，采用UTF-8编解码
import codecs
import re



#分割语料，前90%用于训练，剩余作为测试
with codecs.open("Chinese text corpus.txt",'r',encoding='UTF-8') as corpus:     #打开语料文件
    with codecs.open("train.txt",'w',encoding='UTF-8') as train:
        with codecs.open("test.txt",'w',encoding='UTF-8') as test:
            lines=0
            for line in corpus.readlines():                                     #统计语料总行数
                lines+=1
            print("There are "+str(lines)+" lines in the 'Chinese text corpus.txt'.")
            linesForTraining=int(lines*0.9)                                     #训练语料行数
            print("In these lines, the first "+str(linesForTraining)+" lines are used for training, and the rest are for testing.")
            print("***********************************************************************************************")

            corpus.seek(0)                                                      #指针回至文件头
            counter=0
            for line in corpus.readlines():
                line=line[:-1]
                if len(line)>23:                                                #跳过空行
                    sentence=re.split(r'\s+',line[23:])                         #除去空格，提取出词/词性
                    sentence.remove('')
                    if counter<linesForTraining:                                #处于训练语料部分
                        for i in range(0,len(sentence)):
                            word=re.split('/',sentence[i])                      #除去词性，提取出词
                            train.write(word[0])                                #写入train.txt
                            if i is not len(sentence)-1:
                                train.write(" ")
                        train.write("\r\n")
                    else:                                                       #处于测试语料部分
                        for i in range(0,len(sentence)):
                            word=re.split('/',sentence[i])
                            test.write(word[0])                                 #写入test.txt
                            if i is not len(sentence)-1:
                                test.write(" ")
                        test.write("\r\n")
                counter+=1

        test.close()                                                            #关闭语料文件
    train.close()
corpus.close()
print(">>                          The corpus segmentation has been over.                           <<")
print("***********************************************************************************************")



uniDict={}                                                                      #unigram词典
biDict={}                                                                       #bigram词典
triDict={}                                                                      #trigram词典

with codecs.open("train.txt",'r',encoding='UTF-8') as train:                    #打开训练语料
    with codecs.open("uniDict.txt",'w',encoding='UTF-8') as uniDictFile:        #打开词典文件
        with codecs.open("biDict.txt",'w',encoding='UTF-8') as biDictFile:
            with codecs.open("triDict.txt",'w',encoding='UTF-8') as triDictFile:
                for line in train.readlines():
                    sentence=re.split(r'\s+',line)                              #除去空格，提取出词
                    sentence.remove('')
                    sentence.insert(0,"start")                                  #插入start
                    for i in range(0,len(sentence)):
                        word=sentence[i]
                        if word in uniDict:                                     #生成unigram词典
                            uniDict[word]+=1
                        else:
                            uniDict[word]=1
                        if i<len(sentence)-1:                                   #生成bigram词典
                            word=sentence[i+1]+'/'+word
                            if word in biDict:
                                 biDict[word]+=1
                            else:
                                biDict[word]=1
                            if i<len(sentence)-2:                               #生成trigram词典
                                word=sentence[i+2]+'/'+word
                                if word in triDict:
                                    triDict[word]+=1
                                else:
                                    triDict[word]=1
                            else:
                                word="$/"+word
                                if word in triDict:
                                    triDict[word]+=1
                                else:
                                    triDict[word]=1
                        else:
                            word="$/"+word
                            if word in biDict:
                                biDict[word]+=1
                            else:
                                biDict[word]=1

                dictionary=uniDict.items()
                dictionary=sorted(dictionary)
                for key,value in dictionary:                                    #写入unigram词典文件
                    if key is not "start":
                        uniDictFile.write(key+"\t"+str(value)+"\r\n")

                dictionary=biDict.items()
                dictionary=sorted(dictionary)
                for key,value in dictionary:                                    #写入bigram词典文件
                    biDictFile.write(key+"\t"+str(value)+"\r\n")

                dictionary=triDict.items()
                dictionary=sorted(dictionary)
                for key,value in dictionary:                                    #写入trigram词典文件
                    triDictFile.write(key+"\t"+str(value)+"\r\n")

            triDictFile.close()                                                 #关闭词典文件
        biDictFile.close()
    uniDictFile.close()
train.close()                                                                   #关闭训练语料
print(">>                       The dictionaries of model has been generated.                       <<")
print("***********************************************************************************************")