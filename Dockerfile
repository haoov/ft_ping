FROM debian:trixie
# Installing required tools
RUN apt update -y && apt upgrade -y && apt install -y \
	gcc \
	make \
	wget \
	tar \
	build-essential \
	libncurses5-dev;

# Downloading and installing inetutils-ping-2.0
RUN wget https://ftp.gnu.org/gnu/inetutils/inetutils-2.0.tar.xz && \
	tar -xvf inetutils-2.0.tar.xz && \
	cd inetutils-2.0 && \
	./configure && \
	make && \
	make install;
