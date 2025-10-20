FROM ubuntu:20.04

ENV http_proxy="http://10.136.2.7:3128"
ENV https_proxy="http://10.136.2.7:3128"
ENV HTTP_PROXY="http://10.136.2.7:3128"
ENV HTTPS_PROXY="http://10.136.2.7:3128"

RUN apt-get update && \
   apt-get install -y python3 && \
   rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY script.py .
# from to

VOLUME ["/data_example"]

ENTRYPOINT ["python3", "script.py"]

CMD ["--help"]
# vj;tn gthtnthtcnmcz
# chem vajnee i tyajelee comanda, tem ranishe ee nujno vipolnit
# docker images