FROM python:3.8.6-buster

RUN  pip3 install --upgrade pip

WORKDIR /pfg

COPY . /pfg

RUN pip3 --no-cache-dir install -r requirements.txt     
EXPOSE 5000

ENV PYTHONUNBUFFERED=1

RUN apt install -y tzdata && \
    ln -sf /usr/share/zoneinfo/America/Sao_Paulo \
    /etc/localtime && dpkg-reconfigure --frontend noninteractive tzdata

RUN chmod u+x ./entrypoint.sh
ENTRYPOINT ["./entrypoint.sh"]
