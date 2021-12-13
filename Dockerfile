FROM frolvlad/alpine-gxx
#FROM wearelifetrading/gcc6
MAINTAINER Denis Follereau <dfollereau@amadeus.com>
COPY . /usr/src/myapp
WORKDIR /usr/src/myapp
RUN g++ -o myotherapp otherProc.cpp
RUN g++ -o myapp server.cpp
EXPOSE 8080
ENV PATH /usr/src/myapp:$PATH
#CMD ["./myapp"]
#CMD ["./myotherapp"]
#CMD ["./my_wrapper_script.sh"]
#ENTRYPOINT ["tail", "-f", "/dev/null"]

RUN apk update && apk add --no-cache supervisor 
COPY supervisord.conf /etc/supervisord.conf
CMD ["/usr/bin/supervisord", "-c", "/etc/supervisord.conf"]