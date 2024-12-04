FROM busybox AS source

WORKDIR /docs
COPY test.txt .

FROM node:alpine
EXPOSE 4000



WORKDIR /home/node

COPY --from=source /docs/test.txt .
RUN chown node:node test.txt
USER node
CMD [ "sh" ]

