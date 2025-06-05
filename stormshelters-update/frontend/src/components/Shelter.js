import React, { useState, useEffect } from 'react';
import '../css/Shelter.css';
import ShelterCard from './ShelterModel';
import SortShelter from './SortShelter';
import SearchBar from './SearchBar';
import FilterDropdown from './FilterDropdown';
import Col from 'react-bootstrap/Col';
import Row from 'react-bootstrap/Row';
import Container from 'react-bootstrap/Container';

function Shelters() {
  // Step 1: Define state variable to store shelter data
  const [shelterData, setShelterData] = useState([]);
  const [pageNum, setPageNum] = useState(1);
  const [numInstances, setMetaData ] = useState(1);
  const [selectedSortOption, setSelectedSortOption] = useState(''); // Default sorting option
  const [selectedSortOrder, setSelectedSortOrder] = useState(''); // Default sorting order
  const [closed, setClosed] = useState('');
  const [rating, setRating] = useState('');


  // Step 2: Create an asynchronous function to fetch data
  const fetchData = async (page, sortOption, isClosed, shelterRating) => {
    try {
      let apiUrl = `http://0.0.0.0:8000/shelters?page=${page}&per_page=${9}`;
      
      if (sortOption === 'Name Asc') {
        apiUrl += `&sort=name&order=asc`
      }
      else if (sortOption === 'Name Desc') {
        apiUrl += `&sort=name&order=desc`
      }
      else if (sortOption === 'City Asc') {
        apiUrl += `&sort=city&order=asc`
      }
      else if (sortOption === 'City Desc') {
        apiUrl += `&sort=city&order=desc`
      }
      else if (sortOption === 'Rating Asc') {
        apiUrl += `&sort=rating&order=asc`
      }
      else if (sortOption === 'Rating Desc') {
        apiUrl += `&sort=rating&order=desc`
      }

      if (isClosed !== "" && isClosed != "Filter by Closed") {
        apiUrl += `&closed=${isClosed}`;
      }

      if (shelterRating !== "" && shelterRating != "Filter by Rating") {
        apiUrl += `&rating=${shelterRating}`;
      }

      console.log(apiUrl);

      const response = await fetch(apiUrl);
      if (response.ok) {
        const data = await response.json();
        setShelterData(data.shelters)
        setMetaData(data.meta)
      } else {
        console.error('Error fetching data:', response.status);
      }
    } catch (error) {
      console.error('Error fetching data:', error);
    }
  };

  useEffect(() => {
    fetchData(pageNum, selectedSortOption, closed, rating);
  }, [pageNum, selectedSortOption, closed, rating]);
  
  const handleSortChange = (newSortOption) => {
    setSelectedSortOption(newSortOption);
    setPageNum(1);
  };

  const handleClosedFilter = (value) => {
    setClosed(value);
  };

  const handleRatingFilter = (value) => {
    setRating(value);
  };

  
  // chunk the shelter data into groups of three for grid
  function chunkArray(arr, chunkSize) {
    const chunkedArray = [];
    for (let i = 0; i < arr.length; i += chunkSize) {
      chunkedArray.push(arr.slice(i, i + chunkSize));
    }
    return chunkedArray;
  }
  const chunkedShelterData = chunkArray(shelterData, 1);


  // Step 5: Use React Router to handle navigation
  return (
    <div className="shelters-container">
      <h1>Shelters & Food pantries</h1>
      <Container>
          <Row style={{display: "flex", justifyContent: "space-evenly"}}>
            <Col>
              <SortShelter selectedOption={selectedSortOption} onSortOptionChange={handleSortChange} />
            </Col>
            {/* filtering start*/}
            <Col>
              <FilterDropdown
                title="Filter by Closed"
                items={[
                  "Filter by Closed",
                  "Yes",
                  "No"
                ]}
                onChange={handleClosedFilter}
              />
            </Col>
            <Col>
              <FilterDropdown
                title="Filter by Rating"
                items={[
                  "Filter by Rating",
                  "0-1",
                  "1-2",
                  "2-3",
                  "3-4",
                  "4-5"
                ]}
                onChange={handleRatingFilter}
              />
            </Col>
            {/* filtering end*/}
          </Row>
        </Container>
      <p>Total Instances: {36}</p>
      <SearchBar model='shelter' />

      <div className="shelter-card-container">
        {chunkedShelterData.map((chunk, rowIndex) => (
          <div className="row" key={rowIndex}>
            {chunk.map((id, colIndex) => (
              <div className="col-sm-4" key={colIndex}>
                <ShelterCard shelterData={ id } />
              </div>
            ))}
          </div>
        ))}
      </div>
      <div className="show-more">
      <div className="button-group">
        <button
          onClick={() => {
            setPageNum((prev) => prev - 1);
          }}
          disabled={pageNum === 1}
          className="shelter-button prev-button"
        >
          Previous
        </button>

        <button
          onClick={() => {
            if (shelterData.length > 0) {
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
);
}

export default Shelters;
