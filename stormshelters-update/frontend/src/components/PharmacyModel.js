import React from 'react';
import { Link } from 'react-router-dom';
import '../css/City.css';

const PharmacyCard = ({ pharmacyData }) => {
  const { address, categories, city, distance_m, name, image } = pharmacyData;
  // console.log(image)
  return (  
    <Link style={{textDecoration: 'none'}} to={`/pharmacies/${pharmacyData.id}`}>
    <div className="shelter-card">
      <div className="shelter-image-container">
      <img className="shelter-card-img-top" src={image}></img>
      </div>
      <div className="shelter-card-body">
        {/* <img className="card-img-top" src={im} */}

        <h4 className="shelter-card-title">{name}</h4>
        <ul className="list-group list-group-flush">
          <li className="list-group-item">Name: {name}</li>
          <li className="list-group-item">Address: {address}</li>
          <li className="list-group-item">City: {city}</li>
          <li className="list-group-item">Category: {categories}</li>
          <li className="list-group-item">Distance: {distance_m}</li>
        </ul>
      </div>
    </div>
    </Link>
  );
};

export default PharmacyCard;
