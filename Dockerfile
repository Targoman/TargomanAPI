FROM alpine:3.10.2

LABEL maintainer="S.Mehran M.Ziabary <ziabary@targoman.com>"

WORKDIR /

RUN apk update && apk upgrade && apk add --no-cache bash sed grep coreutils qt5-qtbase qt5-qtwebsockets zlib libxml2
RUN apk add --virtual .build-dependencies --no-cache gcc g++ make qt5-qtbase-dev qt5-qtwebsockets-dev zlib-dev libxml2-dev curl git \
    && ln -s /lib/libz.so /usr/lib/ \
    && bash && cd /tmp \
    && git clone -b $(curl -s https://api.github.com/repos/Targoman/TargomanAPI/releases/latest | grep tag_name | cut -d ':' -f 2 | sed 's/[\", ]//g') --recursive --single-branch https://github.com/Targoman/TargomanAPI.git \
    && cd /tmp/TargomanAPI/TargomanCommon/ && qmake-qt5 PREFIX=/tmp/local && make install
    && cd /tmp/TargomanAPI/QFieldValidator/ && qmake-qt5 PREFIX=/tmp/local && make install
    && cd /tmp/TargomanAPI/E4MT/TextProcessor/ && qmake-qt5 PREFIX=/tmp/local && make -j install
    && cd /tmp/TargomanAPI/E4MT/TextProcessor/fasttext && mkdir build && cd build && cmake .. && make -j install
    && cd /tmp/TargomanAPI/QRestServer/3rdParty/QHttp && qmake-qt5 PREFIX=/tmp/local && make -j install
    && cd /tmp/TargomanAPI/TargomanDBM/ && qmake-qt5 PREFIX=/tmp/local && make -j install
    && cd /tmp/TargomanAPI \
    && qmake-qt5 PREFIX=/tmp/build \
    && make install \
    && mv /tmp/build/bin/* /usr/bin \
    && mv /tmp/build/lib/* /usr/lib \
    && mv /tmp/build/conf/* /etc/TargomanAPI/ \
    && rm -rf /tmp/* \
    && apk del .build-dependencies

COPY ./scripts/E4MT.server.conf ./scripts/tsa.ini /etc/E4MT/

EXPOSE 10000
ENTRYPOINT [ "TargomanAPI", "-c","/etc/TargomanAPI/TargomanAPI.server.conf" ]
