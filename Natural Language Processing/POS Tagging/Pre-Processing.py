# -*- coding: utf-8 -*                                                              #语料为中文，采取UTF-8解编码
import codecs
import re
import sys



reload(sys)
sys.setdefaultencoding('UTF-8')
with codecs.open("Chinese text corpus.txt",'r',encoding='UTF-8') as corpus:         #打开语料文件
    with codecs.open("partitioned corpus.txt",'w',encoding='UTF-8') as result:
        for line in corpus.readlines():
            if len(line)>23:                                                        #跳过空行
                words=re.split(r'\s+',line[23:])                                    #除去空格，提取出词/词性
                words.remove("")
            for i in range(0,len(words)):
                if words[i] is not ' ':
                    result.write(words[i]+' ')
                if "。" in words[i] or "！" in words[i] or "？" in words[i]:         #利用‘。’、‘！’、‘？’进行断句
                    result.write("\r\n")
    result.close()
corpus.close()                                                                      #关闭语料文件
print("***********************************************************************************************")
print(">>                           The corpus partition has been over.                            <<")
print("***********************************************************************************************")



with codecs.open("partitioned corpus.txt",'r',encoding='UTF-8') as corpus:          #打开加工后的语料文件
    with codecs.open("first corpus.txt",'w',encoding='UTF-8') as corpus1:
        with codecs.open("second corpus.txt",'w',encoding='UTF-8') as corpus2:
            with codecs.open("third corpus.txt",'w',encoding='UTF-8') as corpus3:
                with codecs.open("fourth corpus.txt",'w',encoding='UTF-8') as corpus4:
                    with codecs.open("fifth corpus.txt",'w',encoding='UTF-8') as corpus5:
                        with codecs.open("first test.txt",'w',encoding='UTF-8') as test1:
                            with codecs.open("second test.txt",'w',encoding='UTF-8') as test2:
                                with codecs.open("third test.txt",'w',encoding='UTF-8') as test3:
                                    with codecs.open("fourth test.txt",'w',encoding='UTF-8') as test4:
                                        with codecs.open("fifth test.txt",'w',encoding='UTF-8') as test5:
                                            lines=0
                                            for line in corpus.readlines():         #统计语料总行数
                                                lines+=1
                                            print("There are "+str(lines)+" lines in the 'partitioned corpus.txt'.")
                                            print("These lines are divided into five parts, respectively, for training and testing.")
                                            print("***********************************************************************************************")
                                            corpus.seek(0)                          #指针回至文件头
                                            tmp=0
                                            for line in corpus.readlines():
                                                words=re.split(r'\s+',line)
                                                if tmp in range(0,lines/5):         #进行5层划分，为交叉检验做准备
                                                    corpus1.write(line)
                                                    for i in range(0,len(words)):
                                                        word=re.split('/',words[i])
                                                        test1.write(word[0]+' ')
                                                    test1.write("\r\n")
                                                elif tmp in range(lines/5,lines*2/5):
                                                    corpus2.write(line)
                                                    for i in range(0,len(words)):
                                                        word=re.split('/',words[i])
                                                        test2.write(word[0]+' ')
                                                    test2.write("\r\n")
                                                elif tmp in range(lines*2/5,lines*3/5):
                                                    corpus3.write(line)
                                                    for i in range(0,len(words)):
                                                        word=re.split('/',words[i])
                                                        test3.write(word[0]+' ')
                                                    test3.write("\r\n")
                                                elif tmp in range(lines*3/5,lines*4/5):
                                                    corpus4.write(line)
                                                    for i in range(0,len(words)):
                                                        word=re.split('/',words[i])
                                                        test4.write(word[0]+' ')
                                                    test4.write("\r\n")
                                                else:
                                                    corpus5.write(line)
                                                    for i in range(0,len(words)):
                                                        word=re.split('/',words[i])
                                                        test5.write(word[0]+' ')
                                                    test5.write("\r\n")
                                                tmp+=1
                                        test5.close()
                                    test4.close()
                                test3.close()
                            test2.close()
                        test1.close()
                    corpus5.close()
                corpus4.close()
            corpus3.close()
        corpus2.close()
    corpus1.close()
corpus.close()                                                                      #关闭处理后的语料文件
print(">>                            The pre-processing has been over.                             <<")
print("***********************************************************************************************")
