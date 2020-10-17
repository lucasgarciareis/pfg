CREATE DATABASE IF NOT EXISTS
    crud;
create user projetofinal@localhost identified by 'password';
grant all privileges on crud.* to projetofinal@localhost;
USE crud;