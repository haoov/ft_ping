# Pull debian image
FROM debian:latest

# Install requiered packages
RUN	apt update -y && apt install -y \
	gcc \
	clang \
	make \
	wget \
	tar \
	build-essential \
	libncurses5-dev;

# Install inetutils 2.0
RUN	wget https://ftp.gnu.org/gnu/inetutils/inetutils-2.0.tar.xz && \
	tar -xvf inetutils-2.0.tar.xz && \
	cd inetutils-2.0 && \
	./configure && \
	make && \
	make install;
