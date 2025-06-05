FROM python:3.11

RUN apt-get update
RUN apt-get install -y python3
RUN pip3 install --upgrade pip
RUN apt-get install -y libssl-dev

WORKDIR /backend


COPY requirements.txt ./
RUN pip3 install -r requirements.txt
COPY . ./

EXPOSE 5000
EXPOSE 8000
# CMD ["python", "app.py"]
# CMD ["/bin/bash"]
CMD ["gunicorn", "-w", "2", "-b", "0.0.0.0:8000", "app:app"]
