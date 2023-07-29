FROM alpine:3.18.2

ADD build-toolchain.sh /opt/build-toolchain.sh

RUN /opt/build-toolchain.sh

ENV PATH "/opt/toolchain:$PATH"
ENV BUILDROOT "/opt"
WORKDIR /opt
