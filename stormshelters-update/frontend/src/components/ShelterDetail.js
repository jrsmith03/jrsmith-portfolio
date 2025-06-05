import React from 'react';
import { useParams } from 'react-router-dom';
import CityCard from './CityModel';
import PharmacyCard from './PharmacyModel';
import { LoadScript, GoogleMap, Marker } from '@react-google-maps/api';
import ShelterCard from './ShelterModel';
// import shelterData from '../data/shelter-data';
import '../css/CityDetail.css';

const GOOGLE_API_KEY_MAP = 'AIzaSyAP0iwpFt7n8429SqZpI_N-OXxTC5ywfn8';

const ShelterDetail = () => {
    const { id } = useParams();

    var mainURL = `http://0.0.0.0:8000/shelters/${id}`;
    var mainreq = new XMLHttpRequest();
    mainreq.open('GET', mainURL, false); 
    mainreq.send(null);
    var currentShelter = (JSON.parse(mainreq.responseText).shelter);
  
    // Second request
    var newcityreq = new XMLHttpRequest();
    var similarCityURL = `http://0.0.0.0:8000/cities/${Math.floor(Math.random() * 27) + 1}`;
    newcityreq.open('GET', similarCityURL, false); 
    newcityreq.send(null);
    var newCityData = (JSON.parse(newcityreq.responseText).city);
  
    // Third request
    var newpharmreq = new XMLHttpRequest();
    var similarPharmacyURL = `http://0.0.0.0:8000/pharmacies/${Math.floor(Math.random() * 124) + 1}`
    newpharmreq.open('GET', similarPharmacyURL, false); 
    newpharmreq.send(null);
    var newPharmData = (JSON.parse(newpharmreq.responseText).pharmacy);
  
    // Final request
    var newshelterreq = new XMLHttpRequest();
    var similarShelterURL = `http://0.0.0.0:8000/shelters/${Math.floor(Math.random() * 12) + 1}`
    newshelterreq.open('GET', similarShelterURL, false); 
    newshelterreq.send(null);
    var newShelterData = (JSON.parse(newshelterreq.responseText).shelter);


  if (!currentShelter) {
    return <div>No shelter found</div>;
  }

  return (
    <div className="city-detail-container">
      <h1 className="city-title">{currentShelter.name}</h1>
      <div className="instance-media">
      <div className="image-container">
        <img src={currentShelter.image_url} alt={currentShelter.name} className="city-image" />
      </div>

     
        </div>
        <div className="map-container" style={{ margin: '0 auto', width: '600px', height: '400px' }}>
          <LoadScript googleMapsApiKey={GOOGLE_API_KEY_MAP}>
            <GoogleMap
              mapContainerStyle={{ width: '100%', height: '100%' }}
              zoom={15}
              center={{ lat: currentShelter.latitude, lng: currentShelter.longitude }}
            >
              <Marker
                position={{ lat: currentShelter.latitude, lng: currentShelter.longitude }}
              />
            </GoogleMap>
          </LoadScript>
        </div>

      <div className="city-detail-card-container">
        <p className="city-detail-card">Phone: {currentShelter.display_phone}</p>
        <p className="city-detail-card">City: {currentShelter.city}</p>
        <p className="city-detail-card">Rating: {currentShelter.rating}</p>
        <p className="city-detail-card">Address: {currentShelter.display_address}</p>
        <p className="city-detail-card">Closed: No</p>
      </div>


        <div className="shelter-container-container">
          <h1>Related Instances</h1>
          <div className="shelter-card-container">
            <CityCard cityData = {newCityData}/> 
            <PharmacyCard pharmacyData = {newPharmData}/> 
            <ShelterCard shelterData = {newShelterData}/> 
          </div>
        </div>
      </div>
  
  );
};

export default ShelterDetail;
