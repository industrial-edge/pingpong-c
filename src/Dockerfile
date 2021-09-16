
# ------ STAGE 1 (build container) ---------------------

FROM ubuntu:20.04 as build-env

ENV DEBIAN_FRONTEND=noninteractive
RUN apt update && apt install -y build-essential gcc-9 make cmake libssl-dev libjson-c-dev git

# download mqtt client library
RUN git clone https://github.com/eclipse/paho.mqtt.c.git

# compile paho library from sources
RUN cmake -DPAHO_ENABLE_TESTING=FALSE -DPAHO_BUILD_STATIC=TRUE -DPAHO_WITH_SSL=TRUE -DPAHO_BUILD_SAMPLES=FALSE paho.mqtt.c/  
RUN cd paho.mqtt.c/ && make install && cd /

# copy source files into filesystem of container
ADD . /src

# build code
RUN cd /src  && cmake . && make


# ------ STAGE 2 (application container) -----------------

FROM ubuntu:20.04

# install the libssl-dev and libjson-c-dev package
RUN apt update && apt install -y libssl-dev libjson-c-dev

# copy the compiled application and library from the build image to the applciation image
COPY --from=build-env /src/paho_pingpong /
COPY --from=build-env /usr/local/lib/libpaho-mqtt3as.so.1 /usr/local/lib
RUN export LD_LIBRARY_PATH=/usr/local/lib && ldconfig

# run application
CMD ["./paho_pingpong"]
