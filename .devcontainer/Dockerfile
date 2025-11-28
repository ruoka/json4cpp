FROM debian:bookworm-slim AS os

RUN set -ex && \
    apt-get update && \
    apt-get install -y wget git zsh && \
    rm -rf /var/lib/apt/lists/*

RUN set -ex &&\
    echo "deb http://apt.llvm.org/bookworm/ llvm-toolchain-bookworm-19 main" > /etc/apt/sources.list.d/apt.llvm.org.list && \
    wget --no-check-certificate -qO- https://apt.llvm.org/llvm-snapshot.gpg.key | tee /etc/apt/trusted.gpg.d/apt.llvm.org.asc && \
	touch /etc/apt/apt.conf.d/99verify-peer.conf && echo >>/etc/apt/apt.conf.d/99verify-peer.conf "Acquire { https::Verify-Peer false }" && \
    apt-get update && \
    apt-get install --no-install-recommends --yes git cmake make clang-19 lldb-19 lld-19 libc++-19-dev libc++abi-19-dev && \
    rm -rf /var/lib/apt/lists/*

# https://code.visualstudio.com/remote/advancedcontainers/add-nonroot-user
ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID

# Create the user and set zsh as default shell
RUN groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME \
    && usermod --shell /bin/zsh $USERNAME

# [Optional] Set the default user. Omit if you want to keep the default as root.
USER $USERNAME
