FROM python:3.6.12-alpine3.11

RUN apk add --no-cache --update python3-dev  gcc build-base \
    && pip3 install --upgrade pip

WORKDIR /app

COPY . /app

RUN pip3 --no-cache-dir install -r requirements.txt                                                                            

EXPOSE 5000

ENTRYPOINT  ["python3"]

CMD ["-m", "flask", "run", "--host=0.0.0.0"]