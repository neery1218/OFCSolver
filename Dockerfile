FROM ubuntu:16.04
MAINTAINER Neerajen Sritharan <neerajen.sritharan@gmail.com>

RUN apt-get update && apt-get install \
    nano \
    vim \
    wget \
    less \
    make \
    g++ \
    gdb \
    libboost-all-dev \
    openjdk-8-jdk \
    cmake  --yes

RUN apt-get install curl --yes
RUN apt-get install apt-transport-https --yes

# bazel
RUN echo "deb [arch=amd64] http://storage.googleapis.com/bazel-apt stable jdk1.8" | tee /etc/apt/sources.list.d/bazel.list \
  && curl https://bazel.build/bazel-release.pub.gpg | apt-key add -

RUN apt-get update \
  && apt-get install -y bazel \
  && rm -rf /var/lib/apt/lists/*

VOLUME ["/root/src"]

# make sure to start commands in the src folder
WORKDIR /root/src
