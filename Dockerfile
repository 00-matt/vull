# syntax=docker/dockerfile:1

FROM ubuntu:jammy

RUN apt-get update \
 && apt-get install -y \
      ca-certificates \
      gnupg2 \
      software-properties-common \
      wget \
 && wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key \
  | gpg --dearmor - \
  > /usr/share/keyrings/llvm-archive-keyring.gpg \
 && echo 'deb [signed-by=/usr/share/keyrings/llvm-archive-keyring.gpg] https://apt.llvm.org/jammy/ llvm-toolchain-jammy-16 main' > /etc/apt/sources.list.d/llvm.list \
 && wget -O - http://packages.lunarg.com/lunarg-signing-key-pub.asc \
  | gpg --dearmor - \
  > /usr/share/keyrings/lunarg-archive-keyring.gpg \
 && echo 'deb [signed-by=/usr/share/keyrings/lunarg-archive-keyring.gpg] https://packages.lunarg.com/vulkan/ jammy main' > /etc/apt/sources.list.d/lunarg.list \
 && wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc \
  | gpg --dearmor - \
  > /usr/share/keyrings/kitware-archive-keyring.gpg \
 && echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ jammy main' > /etc/apt/sources.list.d/kitware.list \
 && apt-add-repository ppa:ubuntu-toolchain-r/ppa \
 && apt-get update \
 && apt-get install -y \
      clang-16 \
      clang-format-16 \
      clang-tidy-16 \
      clang-tools-16 \
      cmake \
      gcc-12 \
      g++-12 \
      git \
      libclang-16-dev \
      libfreetype-dev \
      libharfbuzz-dev \
      libstdc++-12-dev \
      libx11-dev \
      libxcb-randr0-dev \
      libxcb-util-dev \
      libxcb-xkb-dev \
      libzstd-dev \
      llvm-16-dev \
      ninja-build \
      parallel \
      shaderc \
 && rm -rf \
   /var/lib/apt/lists/* \
 && git clone \
   --depth 1 \
   --branch 0.20 \
   https://github.com/include-what-you-use/include-what-you-use.git \
 && cmake \
   -Biwyu-build \
   -GNinja \
   -DCMAKE_BUILD_TYPE=Release \
   -DCMAKE_PREFIX_PATH=/usr/lib/llvm-16 \
   -DCMAKE_INSTALL_PREFIX=/usr \
   ./include-what-you-use \
 && ninja -Ciwyu-build \
 && ninja -Ciwyu-build install \
 && rm /usr/lib/clang/16/include \
 && cp -r iwyu-build/lib/clang/16/include/ /usr/lib/clang/16/include/ \
 && rm -r include-what-you-use iwyu-build \
 && ln -s /usr/bin/clang-16 /usr/bin/clang \
 && ln -s /usr/bin/clang++-16 /usr/bin/clang++ \
 && ln -s /usr/bin/g++-12 /usr/bin/g++
