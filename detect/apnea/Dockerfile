FROM python:3.8.6-buster

RUN mkdir /usr/src/apnea/

COPY . /usr/src/apnea/

WORKDIR /usr/src/apnea/

RUN pip install -r requirements.txt

RUN apt install -y tzdata && \
    ln -sf /usr/share/zoneinfo/America/Sao_Paulo \
    /etc/localtime && dpkg-reconfigure --frontend noninteractive tzdata

CMD ["python3", "apnea.py"]