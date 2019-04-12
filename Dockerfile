FROM debian:latest
LABEL description="Running a terminal version of Simuino"
MAINTAINER Marco Benecke <m@v7t.de>

RUN apt-get update; apt-get -y upgrade; apt-get -y install ncurses-dev build-essential git
WORKDIR /
RUN git clone https://github.com/bsaxen/simuino.git
WORKDIR /simuino
RUN git clone https://github.com/bsaxen/servuino.git
RUN g++ -o /simuino/simuino simuino.c -lncurses
RUN chmod +x /simuino/simuino

ENTRYPOINT /simuino/simuino

