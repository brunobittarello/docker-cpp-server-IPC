# docker-cpp-server-IPC
A docker container with a socker server that calls a second app inside the container

# Commands

docker build -t cpp-server .
docker run -d -p 8080:8080 cpp-server

# References

https://github.com/dfollereau/clntserver
https://www.youtube.com/watch?v=dniz3nEOEjU
http://veithen.io/2015/01/08/supervisord-redirecting-stdout.html
https://stackoverflow.com/posts/49100302/revisions