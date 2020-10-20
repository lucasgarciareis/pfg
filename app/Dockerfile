FROM python:3.8.6-buster

RUN  pip3 install --upgrade pip

WORKDIR /pfg

COPY . /pfg

RUN pip3 --no-cache-dir install -r requirements.txt     
EXPOSE 5000

RUN chmod u+x ./entrypoint.sh
ENTRYPOINT ["./entrypoint.sh"]