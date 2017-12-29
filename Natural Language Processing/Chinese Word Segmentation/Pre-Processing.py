# -*- coding: utf-8 -*                                                                  #语料为中文，采取UTF-8编解码
import codecs
import re



with codecs.open("Chinese text corpus.txt",'r',encoding='UTF-8') as corpus:             #打开语料文件
    with codecs.open("train.txt",'w',encoding='UTF-8') as train:
        with codecs.open("test.txt",'w',encoding='UTF-8') as test:
            with codecs.open("answer.txt",'w',encoding='UTF-8') as answer:
                lines=0
                for line in corpus.readlines():                                         #统计语料总行数
                    lines+=1
                linesForTraining=int(lines*0.8)                                         #训练集行数
                corpus.seek(0)                                                          #指针回至文件头
                tmp=0
                for line in corpus.readlines():
                    if len(line)>23:                                                    #跳过空行
                        words=re.split(r'\s+',line[23:])                                #除去空格
                        if tmp < linesForTraining:                                      #处于训练集部分
                            for i in range(0,len(words)):
                                word=re.split('/',words[i])
                                train.write(word[0]+" ")                                #生成训练集
                            train.write("\n")
                        else:                                                           #否则
                            for i in range(0,len(words)):
                                word=re.split('/',words[i])
                                answer.write(word[0]+" ")                               #生成分词答案
                                test.write(word[0])                                     #生成测试文本
                            answer.write("\n")
                            test.write("\n")
                    tmp+=1
            answer.close()
        test.close()
    train.close()
corpus.close()                                                                          #关闭语料文件
print("***********************************************************************************************")
print(">>                           The corpus partition has been over.                            <<")
print("***********************************************************************************************")
print("There are "+str(lines)+" lines in the 'Chinese text corpus.txt'.")
print("In these lines, the first "+str(linesForTraining)+" lines are used for training, and the rest are for testing.")
print("***********************************************************************************************")