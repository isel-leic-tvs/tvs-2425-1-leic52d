FROM node:alpine

# some metadata
LABEL mantainer="Jorge Martins"

WORKDIR /nodeapp

COPY [ "nodeapp.js", "package.json", "./" ]

RUN npm install
ENV NODE_PORT=4004

EXPOSE 4004

CMD ["node", "nodeapp.js"]
