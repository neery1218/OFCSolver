apt-get update && apt-get install \
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
    cmake  --yes

apt-get install curl --yes
apt-get install apt-transport-https --yes

# bazel
echo "deb [arch=amd64] http://storage.googleapis.com/bazel-apt stable jdk1.8" | tee /etc/apt/sources.list.d/bazel.list && curl https://bazel.build/bazel-release.pub.gpg | apt-key add -

apt-get update && apt-get install -y bazel && rm -rf /var/lib/apt/lists/*
