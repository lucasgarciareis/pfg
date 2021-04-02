FROM debian:9

RUN apt update

RUN apt install -y tzdata && \
    ln -sf /usr/share/zoneinfo/America/Sao_Paulo \
    /etc/localtime && dpkg-reconfigure --frontend noninteractive tzdata

RUN apt install openntpd

COPY ntpd.conf /etc/ntpd.conf

CMD ["ntpd","-d"]
