# ==== CONFIGURE =====
FROM node:20.5.1 
# set working directory to /app inside the container
WORKDIR /app

# copy app files
COPY package.json .

# ==== BUILD =====
# install dependencies
RUN npm install

COPY . .

# ==== RUN =======
# expose the port on which the app will be running
EXPOSE 3000
# start app
CMD [ "npm", "start" ]