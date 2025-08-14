FROM fedora:42

RUN dnf update -y
RUN dnf install -y gcc vim gcc gcc-c++ clang openssh-server lldb clangd libicu python3 pip cmake perl gdb awk npm git
RUN pip install conan 
RUN pip install ninja
RUN pip install meson

RUN conan profile detect
RUN conan profile detect --name clang
RUN sed -i 's/compiler=gcc/compiler=clang/' root/.conan2/profiles/clang
RUN sed -i 's/compiler.version=.*/compiler.version=20/' root/.conan2/profiles/clang
RUN echo [buildenv] >> root/.conan2/profiles/clang
RUN echo CC=clang >> root/.conan2/profiles/clang
RUN echo CXX=clang++ >> root/.conan2/profiles/clang

RUN sed -i 's#Port 22#Port 22#g' /etc/ssh/sshd_config
RUN sed -i 's#PermitRootLogin.*#PermitRootLogin yes#g' /etc/ssh/sshd_config

RUN ssh-keygen -A
RUN touch ~/.ssh/authorized_keys && chmod 600 ~/.ssh/authorized_keys

RUN mkdir /devenv && mkdir /devenv/workspace && mkdir /devenv/workspace/service

#copy public key into container for autmatic login
COPY id_ed25519.pub /devenv

RUN cat devenv/id_ed25519.pub >> ~/.ssh/authorized_keys

EXPOSE 22 8080

CMD ["/usr/sbin/sshd", "-D"]