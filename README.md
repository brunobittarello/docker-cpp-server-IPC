# docker-cpp-server-IPC
A docker container with a socker server that calls a second app inside the container

# Commands

docker build -t cpp-server .
docker run -d -p 8080:8080 cpp-server