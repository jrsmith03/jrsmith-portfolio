import React from 'react';
import { Link } from 'react-router-dom';
import '../css/Shelter.css';

const ShelterCard = ({ shelterData }) => {
  if (!shelterData || shelterData.length === 0) {
    return <div>No shelter data available</div>;
  }

  var closed_display = shelterData.is_closed ? 'Yes': 'No';
 console.log(closed_display);
  return (
    <Link style={{textDecoration: 'none'}} to={`/shelter/${shelterData.id}`}>

    <div className="shelter-card">
      <div className="shelter-image-container">
        <img src={shelterData.image_url} alt={shelterData.name} className="shelter-card-img-top" />
      </div>
      <div className="shelter-card-body">
        <h4 className="shelter-card-title">{shelterData.name}</h4>
        <ul className="list-group list-group-flush">
          <li className="list-group-item">Phone: {shelterData.display_phone}</li>
          <li className="list-group-item">Rating: {shelterData.rating}</li>
          <li className="list-group-item">Closed: No</li>
          <li className="list-group-item">City: {shelterData.city}</li>
          <li className="list-group-item">Address: {shelterData.display_address }</li>
        </ul>

      </div>
    </div>
    </Link>
  );
};

export default ShelterCard;
