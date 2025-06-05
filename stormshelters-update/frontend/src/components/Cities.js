import React, { useState, useEffect } from 'react';
import Container from 'react-bootstrap/Container';
import Form from 'react-bootstrap/Form';
import '../css/Shelter.css';
import '../css/sort.css'
import CityCard from './CityModel';
import SortCity from './SortCity';
import PharmacyCard from './PharmacyModel';
import ShelterCard from './ShelterModel';
import SearchBar from './SearchBar';
import { useParams } from 'react-router-dom';
import FilterDropdown from './FilterDropdown';
import Col from 'react-bootstrap/Col';
import Row from 'react-bootstrap/Row';

function Cities() {
  // Step 1: Define state variable to store shelter data
  const [cityData, setCityData] = useState([]);
  const [pageNum, setPageNum] = useState(1);
  const [numInstances, setMetaData] = useState(1);
  const [selectedSortOption, setSelectedSortOption] = useState(''); // Default sorting option
  const [selectedSortOrder, setSelectedSortOrder] = useState(''); // Default sorting order
  const [condition, setCondition] = useState('');
  const [precipitation, setPrecipitation] = useState('');


  // Step 2: Create an asynchronous function to fetch data
  const fetchData = async (page, sortOption, cond, precip) => {
    try {
      let apiUrl = `http://0.0.0.0:8000/cities?page=${page}&per_page=${9}`;
      // Determine the sorting order based on the selected option
      if (sortOption === 'City Asc') {
        apiUrl += `&sort=city&order=asc`
      }
      else if (sortOption === 'City Desc') {
        apiUrl += `&sort=city&order=desc`
      }
      else if (sortOption === 'Population Asc') {
        apiUrl += `&sort=pop&order=asc`;
      }
      else if (sortOption === 'Population Desc') {
        apiUrl += `&sort=pop&order=desc`;
      }
      else if (sortOption === 'Temperature Asc') {
        apiUrl += `&sort=temp&order=asc`;
      }
      else if (sortOption === 'Temperature Desc') {
        apiUrl += `&sort=temp&order=desc`;
      }

      if (cond !== "") {
        apiUrl += `&condition=${cond}`;
      }

      if (precip !== "") {
        apiUrl += `&precipitation=${precip}`;
      }

      const response = await fetch(apiUrl);
      if (response.ok) {
        const data = await response.json();
        console.log("The city data: ", data);
        setCityData(data.cities);
        setMetaData(data.meta.count);
        
      } else {
        console.error('Error fetching data:', response.status);
      }
    } catch (error) {
      console.error('Error fetching data:', error);
    }
  };

  useEffect(() => {
    fetchData(pageNum, selectedSortOption, condition, precipitation);
  }, [pageNum, selectedSortOption, condition, precipitation]);

  const handleSortChange = (newSortOption) => {
    setSelectedSortOption(newSortOption);
    setPageNum(1);
  };

  const handleConditionFilter = (value) => {
    setCondition(value);
  };

  const handlePrecipitationFilter = (value) => {
    setPrecipitation(value);
  };


  // chunk the shelter data into groups of three for grid
  const chunkArray = (arr, chunkSize) => {
    const chunkedArray = [];
    for (let i = 0; i < arr.length; i += chunkSize) {
      chunkedArray.push(arr.slice(i, i + chunkSize));
    }
    return chunkedArray;
  };

  const chunkedCityData = chunkArray(cityData, 1);

  // Step 5: Use React Router to handle navigation
  return (
    <Container>
      <div className="shelters-container">
        {/* <div> */}
        <h1>Cities</h1>
        <Container>
          <Row style={{display: "flex", justifyContent: "space-evenly"}}>
            <Col>
              <SortCity selectedOption={selectedSortOption} onSortOptionChange={handleSortChange} />
            </Col>
            {/* filtering start*/}
            <Col>
              <FilterDropdown
                title="Filter by Condition"
                items={[
                  "Filter by Condition",
                  "Partly cloudy",
                  "Sunny"
                ]}
                onChange={handleConditionFilter}
              />
            </Col>
            <Col>
              <FilterDropdown
                title="Filter by Precipitation"
                items={[
                  "Filter by Precipitation",
                  "Light",
                  "Medium",
                  "Heavy"
                ]}
                onChange={handlePrecipitationFilter}
              />
            </Col>
            {/* filtering end*/}
          </Row>
        </Container>
        <p>Total Instances: {numInstances}</p>
        <SearchBar model='city' />


        <div className="shelter-card-container">
          {chunkedCityData.map((chunk, rowIndex) => (
            <div className="row" key={rowIndex}>
              {chunk.map((id, colIndex) => (
                <div className="col-sm-4" key={colIndex}>
                  <CityCard cityData={id} />
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
                if (cityData.length > 0) {
                  setPageNum((prev) => prev + 1);
                }
              }
              }
              disabled={pageNum === Math.ceil(numInstances.count / 9)}

              className="shelter-button next-button"
            >
              Next

            </button>

          </div>
          <h3 class="text-center">Page {pageNum} of {Math.ceil(numInstances / 9)}</h3>
        </div>
      </div>
    </Container>
  );
}

export default Cities;
