import { useParams } from 'react-router-dom';
import CityCard from './CityModel';
import PharmacyCard from './PharmacyModel';
import ShelterCard from './ShelterModel';
import '../css/CityDetail.css';
import '../css/ShelterDetail.css';
import { LoadScript, GoogleMap, Marker } from '@react-google-maps/api';

const GOOGLE_API_KEY_MAP = 'AIzaSyAP0iwpFt7n8429SqZpI_N-OXxTC5ywfn8';

function PharmacyDetail() {
  const { id } = useParams();
  var mainURL = `http://0.0.0.0:8000/pharmacies/${id}`;
  var mainreq = new XMLHttpRequest();
  mainreq.open('GET', mainURL, false); 
  mainreq.send(null);
  var pharmacyData = (JSON.parse(mainreq.responseText).pharmacy);

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
  var similarShelterURL = `http://0.0.0.0:8000/shelters/${Math.floor(Math.random() * 36) + 1}`
  newshelterreq.open('GET', similarShelterURL, false); 
  newshelterreq.send(null);
  var newShelterData = (JSON.parse(newshelterreq.responseText).shelter);



  const { name, city, address, distance_m, categories, longitude, latitude, image } = pharmacyData;

  return (
    <div className="city-detail-container">
      <h1 className="city-title">{name}</h1>
     <div className="instance-media">
     <div className="image-container">
        <img 
          src={image}
          alt="Pharmacy"
          className="city-image"
        />
       </div>
   

      <div className="city-detail-card-container">
        <div className="city-detail-card">
          <p>City: {city}</p>
        </div>
        <div className="city-detail-card">
          <p>Address: {address}</p>
        </div>
        <div className="city-detail-card">
          <p>Distance: {distance_m}</p>
        </div>
        <div className="city-detail-card">
          <p>Category: {categories}</p>
        </div>
      </div>

      <div className="map-container" style={{ margin: '0 auto', width: '600px', height: '400px' }}>
            <LoadScript googleMapsApiKey={GOOGLE_API_KEY_MAP}>
              <GoogleMap
                mapContainerStyle={{ width: '100%', height: '100%' }}
                zoom={15}
                center={{ lat: latitude, lng: longitude }}
              >
                <Marker
                  position={{ lat: latitude, lng: longitude }}
                />
              </GoogleMap>
            </LoadScript>
            </div>
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
}

export default PharmacyDetail;
