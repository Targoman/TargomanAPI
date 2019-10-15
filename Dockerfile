FROM alpine:3.10.2

LABEL maintainer="S.Mehran M.Ziabary <ziabary@targoman.com>"

WORKDIR /

RUN apk update && apk upgrade && apk add --no-cache bash sed grep coreutils qt5-qtbase qt5-qtwebsockets zlib libxml2 hiredis curl
RUN apk add --virtual .build-dependencies --no-cache gcc g++ make qt5-qtbase-dev qt5-qtwebsockets-dev zlib-dev hiredis-dev libxml2-dev curl-dev cmake git \
    && ln -s /lib/libz.so /usr/lib/ \
    && bash && cd /tmp \
    && git clone -b $(curl -s https://api.github.com/repos/Targoman/TargomanAPI/releases/latest | grep tag_name | cut -d ':' -f 2 | sed 's/[\", ]//g') --recursive --single-branch https://github.com/Targoman/TargomanAPI.git \
    && sed -i "s/isystem/I/g" /usr/lib/qt5/mkspecs/common/gcc-base.conf \
    && cd /tmp/TargomanAPI/ \
    && ./build.sh \
    && mv /tmp/build/conf/* /etc/TargomanAPI/ \
    && rm -rf /tmp/* \
    && apk del .build-dependencies

COPY ./scripts/E4MT.server.conf ./scripts/tsa.ini /etc/E4MT/

EXPOSE 10000
ENTRYPOINT [ "TargomanAPI", "-c","/etc/TargomanAPI/TargomanAPI.server.conf" ]
