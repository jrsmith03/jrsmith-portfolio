import React from 'react';
import { Link } from 'react-router-dom';
import '../css/Shelter.css';



const CityCard = ({ cityData }) => {
  const { id, name, pop, temp_in_f, wind_mph, cond, precip_in, image } = cityData;
  return (
    <Link style={{textDecoration: 'none'}} to ={`/cities/${id}`}>

    <div className="shelter-card">
      <div className="shelter-image-container">
      <img className="shelter-card-img-top" src={image}></img>
      {/* <img className="shelter-card-img-top" src={url}></img> */}
      </div>
      <div className="shelter-card-body">
        {/* <Link to={`/city/${id}`} > */}
          <h2 className="shelter-card-title">{name}</h2>
        <ul className="list-group list-group-flush">
          <li className="list-group-item">Population: {pop}</li>
          <li className="list-group-item">Temperature: {temp_in_f}°F</li>
          <li className="list-group-item">Wind: {wind_mph}mph</li>
          <li className="list-group-item">Conditions: {cond}</li>
          <li className="list-group-item">Precipitation: {precip_in} inches</li>
        </ul>
      </div>
    </div>
    </Link>
  );
};

export default CityCard;
