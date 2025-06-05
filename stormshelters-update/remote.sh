git pull
cd ./backend
sudo docker kill $(sudo docker ps -q)
sudo docker image rm backend
sudo docker build --tag backend --file ./Dockerfile .
sudo docker run --rm --network=host -i -t -v /home/ubuntu/stormshelters/backend/:/usr/python backend
