mysql -e "CREATE DATABASE IF NOT EXISTS
    crud;" -uroot -ppassword 
mysql -e "create user 'projetofinal'@'%' identified by 'password';" -uroot -ppassword 
mysql -e "grant all privileges on crud.* to 'projetofinal'@'%';" -uroot -ppassword 
mysql -e "ALTER USER 'projetofinal'@'%' IDENTIFIED WITH mysql_native_password BY 'password';" -uroot -ppassword 
mysql -e "USE crud;" -uroot -ppassword 
mysql -e "FLUSH privileges;" -uroot -ppassword 
