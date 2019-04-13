FROM ubuntu:16.04
MAINTAINER Neerajen Sritharan <neerajen.sritharan@gmail.com>

RUN apt-get update
RUN apt-get install nano vim wget less --yes
RUN apt-get install make g++ gdb --yes
RUN apt-get install libboost-all-dev  --yes

VOLUME ["/root/src"]

# make sure to start commands in the src folder
WORKDIR /root/src
