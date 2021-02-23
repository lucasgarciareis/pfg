FROM mysql
ENV MYSQL_ROOT_PASSWORD password
COPY ./script.sql /docker-entrypoint-initdb.d
RUN apt install -y tzdata && \
    ln -sf /usr/share/zoneinfo/America/Sao_Paulo \
    /etc/localtime && dpkg-reconfigure --frontend noninteractive tzdata