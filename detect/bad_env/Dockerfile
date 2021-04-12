FROM python:3.8.6-buster

RUN mkdir /usr/src/bad_env/

COPY . /usr/src/bad_env/

WORKDIR /usr/src/bad_env/

RUN pip install -r requirements.txt

RUN apt install -y tzdata && \
    ln -sf /usr/share/zoneinfo/America/Sao_Paulo \
    /etc/localtime && dpkg-reconfigure --frontend noninteractive tzdata

CMD ["python3", "bad_env.py"]