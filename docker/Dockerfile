FROM debian:trixie

RUN apt-get update && apt-get install -y git build-essential python3 cmake
RUN git clone https://gitlab.com/nsnam/ns-3-dev.git -b ns-3.44
RUN git -C /ns-3-dev clone https://github.com/lucasoshiro/GridGooseSV.git contrib/GridGooseSV/
WORKDIR /ns-3-dev
RUN ./ns3 configure --build-profile=optimized --enable-examples --enable-tests
RUN ./ns3 build
