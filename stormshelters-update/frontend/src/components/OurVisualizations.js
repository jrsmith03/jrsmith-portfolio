import PharmaciesPerCity from "./PharmaciesPerCity";
import Container from "react-bootstrap/esm/Container"
import ShelterRatings from "./ShelterRatings";
import WindTempScatter from "./WindTempScatter"

const OurVisualizations = () => {
    return (
      <>
        <Container className="d-flex justify-content-center flex-column">
          <Container className="container text-center mt-5 mb-4">
            <h1>Our Visualizations</h1>
            <Container className="container text-center mt-3 mb-4">
              <PharmaciesPerCity />
              <ShelterRatings />
              <WindTempScatter />
            </Container>
          </Container>
        </Container>
      </>
    );
  };
  
  export default OurVisualizations;
  