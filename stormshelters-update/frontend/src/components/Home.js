import React, { useEffect } from 'react';
import { Link } from 'react-router-dom';
import SearchBar from './SearchBar';
import '../css/Home.css';


function Home() {

  useEffect(() => {
    // Add a click event listener to the "Find Resources" button
    const scrollButton = document.getElementById('scrollButton');
    const shelterCardContainer = document.getElementById('shelterCardContainer');

    if (scrollButton && shelterCardContainer) {
      scrollButton.addEventListener('click', () => {
        shelterCardContainer.scrollIntoView({ behavior: 'smooth' });
      });
    }
  }, []);
  
  return (
    <div className="home-container">
        <div className="bg">
        <div className="text-container">

          <h1>Prepare for the Storms.</h1>
          <button className="button" id="scrollButton">Find Resources</button>
          <div className='home-search'>
            <h2>Search for resources</h2>
            <SearchBar model={'all'} />
          </div>
        </div>

      </div>
        
      <div className="shelter-card-container" id="shelterCardContainer">
        {/* Card for Cities */}
        <Link style={{textDecoration: 'none'}} to={`/cities`}>

        <div className="shelter-card">
        <img class="shelter-image-container" src="https://ane4bf-datap1.s3-eu-west-1.amazonaws.com/wmocms/s3fs-public/styles/featured_media_detail/public/23613146140_6edfea579f_k.jpg?m0oNCnmjJAswk0CJrWZ.iW.ge6XbrfR1&itok=YBQjaq77" alt="Food and Pharmacies Image"/>
          <div className="shelter-card-body">
            <h3 className="shelter-card-title">Cities</h3>
            <p>
              View the weather conditions of your city to see if a storm is in the forecast. 
            </p>
          </div>
          <div className="button-container">
          </div>
        </div>
        </Link>

        {/* Card for Shelters */}
        <Link style={{textDecoration: 'none'}} to={`/shelter`}>

        <div className="shelter-card">
        <img class="shelter-image-container" src='https://i.cbc.ca/1.5499955.1584413818!/fileImage/httpImage/image.jpg_gen/derivatives/16x9_780/welcome-inn.jpg'/>
          <div className="shelter-card-body">
            <h3 className="shelter-card-title">Shelters & Food</h3>
            <p>
              View the areas to take refuge in Harris County and the highest-rated locations where you can stock up for the next storm or replenish your supplies while you recover. Filter by address to find the most convenient location for you.
            </p>
          </div>
          <div className="button-container">
          </div>
        </div>
        </Link>

        {/* Card for Food and Pharmacies */}
        <Link style={{textDecoration: 'none'}} to={`/pharmacies`}>
        <div className="shelter-card">
        <img class="shelter-image-container" src='https://medicopharmacyrx.com/wp-content/uploads/2021/07/types-of-pharmacy.jpg' alt='Pharmacy Image'/>
          <div className="shelter-card-body">
            <h3 className="shelter-card-title">Pharmacies</h3>
            <p>
              Find where you can purchase essential medical supplies closest to you.
            </p>
          </div>
        </div>
        </Link>
      </div>
      
  

    </div>


  );
}

export default Home;
