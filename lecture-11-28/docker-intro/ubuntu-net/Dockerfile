FROM ubuntu:latest
LABEL mantainer="Jorge Martins"
RUN apt update && apt install -y net-tools && \
                  apt install -y dnsutils && apt install -y inetutils-ping
ENTRYPOINT [ "bash" ]

