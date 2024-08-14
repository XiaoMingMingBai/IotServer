## mysql 使用

### 1. 用户操作

```shell
# 登录
mysql -u root -p

# 登录用户
mysql -u username -p

# 远程登录
mysql -h host -u username -p
```

```sql
-- 显示用户
SELECT user,host FROM mysql.user;
-- 创建用户
CREATE USER 'username'@'host' IDENTIFIED BY 'password';
-- 创建远程用户
CREATE USER 'username'@'%' IDENTIFIED BY 'password';
-- 删除用户
DROP USER 'username'@'host';
-- 修改密码
SET PASSWORD FOR 'username'@'host' = PASSWORD('newpassword');

-- 查看权限
SHOW GRANTS FOR 'username'@'host';

-- 授权所有数据库
GRANT ALL PRIVILEGES ON *.* TO 'username'@'host' IDENTIFIED BY 'password';
-- 授权指定数据库
GRANT ALL PRIVILEGES ON database.* TO 'username'@'host' IDENTIFIED BY 'password';
-- 授权指定表
GRANT ALL PRIVILEGES ON database.table TO 'username'@'host' IDENTIFIED BY 'password';
-- 授权指定字段
GRANT SELECT, INSERT, UPDATE, DELETE ON database.table(field1, field2) TO 'username'@'host' IDENTIFIED BY 'password';



```

### 2. 数据库操作

```sql
-- 显示数据库
SHOW DATABASES;
-- 使用数据库
USE database_name;
-- 创建数据库
CREATE DATABASE database_name;
-- 删除数据库
DROP DATABASE database_name;



```

### 3. 表操作

```sql
-- 显示表
SHOW TABLES;
--查看表内容
SELECT * FROM table_name;
-- 显示表结构
DESC table_name;
-- 创建表
CREATE TABLE table_name (field1 type, field2 type, ...);
-- 删除表
DROP TABLE table_name;
-- 插入数据
INSERT INTO table_name (field1, field2, ...) VALUES (value1, value2, ...);
-- 查询数据
SELECT field1, field2, ... FROM table_name WHERE condition;
-- 更新数据
UPDATE table_name SET field1=value1, field2=value2 WHERE condition;
-- 删除数据
DELETE FROM table_name WHERE condition;
-- 删除表数据
TRUNCATE TABLE table_name;
-- 添加字段
ALTER TABLE table_name ADD field type;
-- 删除字段
ALTER TABLE table_name DROP field;
-- 修改字段
ALTER TABLE table_name MODIFY field type;
-- 修改字段名
ALTER TABLE table_name CHANGE old_field new_field type;
-- 修改字段位置
ALTER TABLE table_name MODIFY field type FIRST;
ALTER TABLE table_name MODIFY field type AFTER field;
-- 修改表名
ALTER TABLE old_table_name RENAME new_table_name;
-- 修改表引擎
ALTER TABLE table_name ENGINE=InnoDB;
-- 修改表字符集
ALTER TABLE table_name CHARACTER SET utf8;
-- 修改表注释
ALTER TABLE table_name COMMENT 'comment';

```