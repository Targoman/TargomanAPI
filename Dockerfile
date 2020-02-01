FROM opensuse/leap:15.1

LABEL maintainer="S.Mehran M.Ziabary <ziabary@targoman.com>"

WORKDIR /

RUN zypper install -y libhiredis0_13 libQt5Sql5-mysql libQt5WebSockets5
COPY ~/

VOLUME . /etc/TargomanAPI

EXPOSE 10000
ENTRYPOINT [ "TargomanAPI", "-c","/etc/TargomanAPI/api.server.conf" ]
