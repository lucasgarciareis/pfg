FROM python:3.8.6-buster
RUN mkdir /usr/src/broker/
COPY . /usr/src/broker/
WORKDIR /usr/src/broker/
RUN pip install -r requirements.txt
CMD ["python", "broker.py"]