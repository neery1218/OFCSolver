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
    linux-tools-common \
    linux-tools-generic \
    git \
    cmake  --yes

RUN apt-get install curl --yes
RUN apt-get install apt-transport-https --yes

# bazel
RUN echo "deb [arch=amd64] http://storage.googleapis.com/bazel-apt stable jdk1.8" | tee /etc/apt/sources.list.d/bazel.list \
  && curl https://bazel.build/bazel-release.pub.gpg | apt-key add -

RUN apt-get update \
  && apt-get install -y bazel \
  && rm -rf /var/lib/apt/lists/*

RUN mkdir /root/.ssh/
ADD docker-deploy /root/.ssh/id_rsa

# Create known_hosts
RUN touch /root/.ssh/known_hosts
# Add bitbuckets key
RUN ssh-keyscan github.com  >> /root/.ssh/known_hosts

RUN git clone -b research git@github.com:neery1218/OFCSolver.git

