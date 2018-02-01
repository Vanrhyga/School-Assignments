[TOC]

### SQL简介

**SQL，指结构化查询语言，用于访问和处理数据库。**

RDBMS，指关系型数据库管理系统，其数据被存储在表（tables）中。

表是相关的数据项的集合，由行和列组成。



### SQL语法

一个数据库通常包含一个或多个表。

每个表由一个名字标识，包含带有数据的记录（行）。

下面的语句从 Persons 表中选取 LastName 列的数据：

~~~sql
SELECT LastName FROM Persons
~~~

**SQL对大小写不敏感！**

可以把SQL分为两个部分：数据操作语言（DML）和数据定义语言（DDL）。

查询和更新指令构成了SQL的DML部分：

> SELECT - 从数据库表中获取数据
>
> UPDATE - 更新数据库表中的数据
>
> DELETE - 从数据库表中删除数据
>
> INSERT INTO - 向数据库表中插入数据

DDL部分使我们有能力创建或删除表格：

> CREATE DATABASE - 创建新数据库
>
> ALTER DATABASE - 修改数据库
>
> CREATE TABLE - 创建新表
>
> ALTER TABLE - 变更数据库表
>
> DROP TABLE - 删除表
>
> CREATE INDEX - 创建索引
>
> DROP INDEX - 删除索引



### SQL SELECT 语句

SELECT语句用于从表中选取数据，结果被存储在一个结果集中。

SQL SELECT 语法：

~~~sql
SELECT 列名称 FROM 表名称
~~~

以及：

~~~sql
SELECT * FROM 表名称
~~~

例如，从 Persons 数据库表中获取 "LastName" 和 "FirstName" 列的内容：

~~~sql
SELECT LastName,FirstName FROM Persons
~~~

现在我们希望从 Persons 表中选取所有的列，可以使用符号 * 取代列的名称：

~~~sql
SELECT * FROM Persons
~~~

**提示**：* 是选取所有列的快捷方式。



### SQL SELECT DISTINCT 语句

在表中，可能会包含重复值。不过，有时希望仅仅列出不同的值，关键词 DISTINCT 用于返回唯一不同的值：

~~~sql
SELECT DISTINCT 列名称 FROM 表名称
~~~



### SQL WHERE 子句

**WHERE 子句用于规定选择的标准**

如需有条件地从表中选取数据，可将 WHERE 子句添加到 SELECT 语句：

~~~sql
SELECT 列名称 FROM 表名称 WHERE 列 运算符 值
~~~

下面的运算符可在 WHERE 子句中使用：

| 操作符     | 描述     |
| ------- | ------ |
| =       | 等于     |
| <>      | 不等于    |
| >       | 大于     |
| <       | 小于     |
| >=      | 大于等于   |
| <=      | 小于等于   |
| BETWEEN | 在某个范围内 |
| LIKE    | 搜索某种模式 |

**注释：**某些版本的SQL中，<> 可写为 != 。

如果只希望选取居住在城市 "Beijing" 中的人：

~~~sql
SELECT * FROM Persons WHERE City='Beijing'
~~~

**SQL使用单引号来环绕文本值（大部分数据库系统也接受双引号）。若是数值，请不要使用引号。**

*文本值：*

> 这是正确的：
>
> SELECT * FROM Persons WHERE FirstName='Bush'

> 这是错误的：
>
> SELECT * FROM Persons WHERE FirstName=Bush

*数值：*

> 这是正确的：
>
> SELECT * FROM Persons WHERE Year>1965

> 这是错误的：
>
> SELECT * FROM Persons WHERE Year>'1965'



### SQL AND&OR 运算符

**AND 和 OR 运算符基于一个以上的条件对记录进行过滤。**

AND 和 OR 可在 WHERE 子语句中把两个或多个条件结合起来。

如果第一个条件和第二个条件都成立，则 AND 运算符显示一条记录。

如果第一个条件和第二个条件只要有一个成立，则 OR 运算符显示一条记录。

使用 AND 来显示所有姓为 "Carter" 并且名为 "Thomas" 的人：

~~~sql
SELECT * FROM Persons WHERE FirstName='Thomas' AND LastName='Carter'
~~~

使用 OR 来显示所有姓为 "Carter" 或者名为 "Thomas" 的人：

~~~sql
SELECT * FROM Persons WHERE FirstName='Thomas' OR LastName='Carter'
~~~

也可以把 AND 和 OR 结合起来：

~~~sql
SELECT * FROM Persons WHERE (FirstName='Thomas' OR FirstName='William') AND LastName='Carter'
~~~



### SQL ORDER BY 子句

**ORDER BY 语句用于对结果集进行排序**

默认按照升序对记录进行排序。如果希望按照降序，可以使用 DESC 关键字。

以字母顺序显示公司名称：

~~~sql
SELECT Company,OrderNumber FROM Orders ORDER BY Company
~~~

以字母顺序显示公司名称，并以数字顺序显示顺序号：

~~~sql
SELECT Company,OrderNumber FROM Orders ORDER BY Company,OrderNumber
~~~

以逆字母顺序显示公司名称：

~~~sql
SELECT Company,OrderNumber FROM Orders ORDER BY Company DESC
~~~

以逆字母顺序显示公司名称，并以数字顺序显示顺序号：

~~~sql
SELECT Company,OrderNumber FROM Orders ORDER BY Company DESC,OrderNumber ASC
~~~



### SQL INSERT INTO 语句

INSERT INTO 语句用于向表格中插入新的行。

语法：

~~~sql
INSERT INTO 表名称 VALUES (值1,值2,....)
~~~

也可以指定所要插入数据的列：

~~~sql
INSERT INTO table_name (列1,列2,...) VALUES (值1,值2,....)
~~~

例如：

~~~sql
INSERT INTO Persons VALUES ('Gates','Bill','Xuanwumen 10','Beijing')

INSERT INTO Persons (LastName,Address) VALUES('Wilson','Champs-Elysees')
~~~



### SQL UPDATE 语句

UPDATE 语句用于修改表中的数据。

语法：

~~~sql
UPDATE 表名称 SET 列名称 = 新值 WHERE 列名称 = 某值
~~~

例如：

~~~sql
UPDATE Persons SET FirstName='Fred' WHERE LastName='Wilson'
~~~

~~~sql
UPDATE Persons SET Address='Zhongshan 23',City='Nanjing' WHERE LastName='Wilson'
~~~



### SQL DELETE 语句

DELETE 语句用于删除表中的行。

语法：

~~~sql
DELETE FROM 表名称 WHERE 列名称 = 值
~~~

例如：

~~~sql
DELETE FROM Persons WHERE LastName='Wilson'
~~~

可以在不删除表的情况下删除所有的行，这意味着表的结构、属性和索引都是完整的：

~~~sql
DELETE FROM table_name
~~~

或者：

~~~sql
DELETE * FROM table_name
~~~



