FROM python:3.8.6-buster

RUN mkdir /usr/src/bot/

COPY . /usr/src/bot/

WORKDIR /usr/src/bot/

RUN pip install -r requirements.txt

RUN apt install -y tzdata && \
    ln -sf /usr/share/zoneinfo/America/Sao_Paulo \
    /etc/localtime && dpkg-reconfigure --frontend noninteractive tzdata

CMD ["python3", "bot.py"]