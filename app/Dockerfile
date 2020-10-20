FROM python:3.8.6-buster

RUN  pip3 install --upgrade pip

WORKDIR /pfg

COPY . /pfg

RUN pip3 --no-cache-dir install -r requirements.txt     
RUN flask db init 
RUN flask db migrate 
RUN flask db upgrade                                                                    

EXPOSE 5000

CMD ["flask", "run", "--host=0.0.0.0"]