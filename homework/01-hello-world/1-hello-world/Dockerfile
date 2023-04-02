FROM hello-world-base:latest

WORKDIR /app
RUN cd /app/build && \
    cmake --build . --target hello-world

CMD ["/app/build/1-hello-world/hello-world"]
