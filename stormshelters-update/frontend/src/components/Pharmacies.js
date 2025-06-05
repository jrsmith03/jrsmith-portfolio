import React, { useState, useEffect } from 'react';
import '../css/Pharmacy.css';
import PharmacyCard from './PharmacyModel';
import SortPharmacy from './SortPharmacy';
import SearchBar from './SearchBar';
import FilterDropdown from './FilterDropdown';
import Col from 'react-bootstrap/Col';
import Row from 'react-bootstrap/Row';
import Container from 'react-bootstrap/Container';

function Pharmacies() {
  // Step 1: Define state variable to store city data
  const [pharmacyData, setPharmacyData] = useState([]);
  const [pageNum, setPageNum] = useState(1);
  const [numInstances, setMetaData ] = useState(1);
  const [selectedSortOption, setSelectedSortOption] = useState(''); // Default sorting option
  const [selectedSortOrder, setSelectedSortOrder] = useState(''); // Default sorting order
  const [name, setName] = useState('');
  const [city, setCity] = useState('');


  // Step 2: Create an asynchronous function to fetch data
  const fetchData = async (page, sortOption, pharmName, pharmCity) => {
    try {
      let apiUrl = `http://0.0.0.0:8000/pharmacies?page=${page}`
      
      if (sortOption === 'Name Asc') {
        apiUrl += `&sort=name&order=asc`
      }
      else if (sortOption === 'Name Desc') {
        apiUrl += `&sort=name&order=desc`
      }
      else if (sortOption === 'City Asc'){
        apiUrl += `&sort=city&order=asc`
      }
      else if (sortOption === 'City Desc'){
        apiUrl += `&sort=city&order=desc`
      }
      else if (sortOption === 'Distance Asc'){
        apiUrl += `&sort=dist&order=asc`
      }
      else if (sortOption === 'Distance Desc'){
        apiUrl += `&sort=dist&order=desc`
      }

      if (pharmName !== "" && pharmName != "Filter by Name") {
        apiUrl += `&name=${pharmName}`;
      }

      if (pharmCity !== "" && pharmCity != "Filter by City") {
        apiUrl += `&city=${pharmCity}`;
      }

      const response = await fetch(apiUrl);
      if (response.ok) {
        const data = await response.json();
        setPharmacyData(data.pharmacies)
        setMetaData(data.meta)
      } else {
        console.error('Error fetching data:', response.status);
      }
    } catch (error) {
      console.error('Error fetching data:', error);
    }
  };

  useEffect(() => {
    fetchData(pageNum, selectedSortOption, name, city);
  }, [pageNum, selectedSortOption, name, city]);
  
  const handleSortChange = (newSortOption) => {
    setSelectedSortOption(newSortOption);
    setPageNum(1);
  };

  const handleNameFilter = (value) => {
    setName(value);
  };

  const handleCityFilter = (value) => {
    setCity(value);
  };

  // Step 4: Render data in the desired format
  const chunkArray = (arr, chunkSize) => {
    const chunkedArray = [];
    for (let i = 0; i < arr.length; i += chunkSize) {
      chunkedArray.push(arr.slice(i, i + chunkSize));
    }
    return chunkedArray;
  };

  const chunkedPharmacyData = chunkArray(pharmacyData, 3);

  // Step 5: Use React Router to handle navigation
  return (
    <Container>
      <div className="pharmacies-container">
        {/* <div> */}
        <h1>Pharmacies</h1>
        <Container>
          <Row style={{display: "flex", justifyContent: "space-evenly"}}>
            <Col>
              <SortPharmacy selectedOption={selectedSortOption} onSortOptionChange={handleSortChange} />
            </Col>
            {/* filtering start*/}
            <Col>
              <FilterDropdown
                title="Filter by Name"
                items={[
                  "Filter by Name",
                  "Brookshire Brothers Pharmacy",
                  "Campbell's Compounding",
                  "CVS Pharmacy",
                  "East End Pharmacy",
                  "H-E-B Pharmacy",
                  "Iridium Pharmacy",
                  "Kelsey-Seybold Pharmacy",
                  "Kroger Pharmacy",
                  "Market Street Pharmacy",
                  "Mid Town Specialty RX Pharmacy",
                  "Randalls Pharmacy",
                  "Tayco Farms",
                  "Tidwell Professional Pharmacy",
                  "Walgreens",
                  "Walmart Pharmacy",
                  "Westside Drug"
                ]}
                onChange={handleNameFilter}
              />
            </Col>
            <Col>
              <FilterDropdown
                title="Filter by City"
                items={[
                  "Filter by City",
                  "Aldine",
                  "Atascocita",
                  "Barrett",
                  "Bellaire",
                  "Bunker Hill Village",
                  "Cloverleaf",
                  "Cypress",
                  "Deer Park",
                  "El Lago",
                  "Galena Park",
                  "Hedwig Village",
                  "Highlands",
                  "Houston",
                  "Huffman",
                  "Humble",
                  "Jersey Village",
                  "Katy",
                  "Kingwood",
                  "La Porte",
                  "Morgans Point",
                  "Pasadena",
                  "Sheldon",
                  "Southside Place",
                  "Spring",
                  "Tomball",
                  "Waller",
                  "Webster"
                ]}
                onChange={handleCityFilter}
              />
            </Col>
            {/* filtering end*/}
          </Row>
        </Container>
      <p>Total Instances: {124}</p>
      <SearchBar model='pharmacies' />


      <div className="card-container">
        {chunkedPharmacyData.map((chunk, rowIndex) => (
          <div className="row" key={rowIndex}>
            {chunk.map((id, colIndex) => (
              <div className="col-sm-4" key={colIndex}>
                <PharmacyCard pharmacyData={id} />
              </div>
            ))}
          </div>
        ))}
      </div>

      <div className="show-more">
        <div className="button-group">
        <button
            onClick={() => setPageNum((prev) => prev - 1)}
        
            disabled={pageNum === 1}
            className="shelter-button prev-button"
          >
            Previous
          </button>

          <button
            onClick={() => {
              if (pharmacyData.length > 0) {
                setPageNum((prev) => prev + 1);
              }
            }}
            disabled={pageNum === Math.ceil(numInstances.count / 9)}

            className="shelter-button next-button"
          >
            Next
            
          </button>

        </div>
        <h3 class="text-center">Page {pageNum} of {Math.ceil(numInstances.count / 9)}</h3>
      </div>
    </div>
    </Container>
    
  );
}

export default Pharmacies;
