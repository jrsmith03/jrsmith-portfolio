DROP DATABASE IF EXISTS model;
CREATE DATABASE model;

USE model;

CREATE TABLE cities (
    id int AUTO_INCREMENT,
    name varchar(255), 
    pop varchar(255), 
    temp_in_f float, 
    wind_mph float, 
    cond varchar(255), 
    precip_in float, 
    image varchar(255),
    PRIMARY KEY (id)
);

CREATE TABLE pharmacies (
    id int AUTO_INCREMENT,
    name varchar(255), 
    city varchar(255), 
    address varchar(255), 
    distance_m int, 
    categories varchar(255), 
    longitude float, 
    latitude float,
    website varchar(255),
    image varchar(255),
    PRIMARY KEY (id)
);

CREATE TABLE shelters (
    id int AUTO_INCREMENT,
    name varchar(255), 
    url varchar(255), 
    is_closed varchar(255), 
    city varchar(255),
    rating int, 
    display_address varchar(255), 
    display_phone varchar(255), 
    image_url varchar(255), 
    latitude float, 
    longitude float,
    PRIMARY KEY (id)
);


