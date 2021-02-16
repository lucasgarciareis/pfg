CREATE DATABASE IF NOT EXISTS crud;
create user 'projetofinal'@'%' identified by 'password'; 
grant all privileges on crud.* to 'projetofinal'@'%';
ALTER USER 'projetofinal'@'%' IDENTIFIED WITH mysql_native_password BY 'password';
USE crud;
FLUSH privileges;
