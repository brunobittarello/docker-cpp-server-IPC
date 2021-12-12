FROM frolvlad/alpine-gxx
#FROM wearelifetrading/gcc6
MAINTAINER Denis Follereau <dfollereau@amadeus.com>
COPY . /usr/src/myapp
WORKDIR /usr/src/myapp
RUN g++ -o myotherapp otherProc.cpp
RUN g++ -o myapp server.cpp
EXPOSE 8080
ENV PATH /usr/src/myapp:$PATH
CMD ["./myapp"]
#CMD ["./myotherapp"]
