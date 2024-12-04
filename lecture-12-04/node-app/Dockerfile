FROM node:alpine

# some metadata
#LABEL mantainer="Jorge Martins"

#define work directory eventually creating it
WORKDIR /nodeapp

COPY [ "nodeapp.js", "package.json", "./" ]

RUN npm install

CMD ["node", "nodeapp.js"]
