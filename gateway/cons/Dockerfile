FROM python:3.8.6-buster
RUN mkdir /usr/src/cons/
COPY . /usr/src/cons/
WORKDIR /usr/src/cons/
RUN pip install -r requirements.txt
CMD ["python", "cons.py"]