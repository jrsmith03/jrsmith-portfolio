import React, { useEffect, useState } from "react";
import axios from "axios";
import Container from "react-bootstrap/Container";
import Row from 'react-bootstrap/Row';
import Col from 'react-bootstrap/Col';
import { BarChart, Bar, XAxis, YAxis, Tooltip, ResponsiveContainer } from 'recharts';


const PharmaciesPerCity = () => {
    const [data, setData] = useState(null);
    
    useEffect(() => {
        const fetchData = async () => {
            const cities = new Map();

            // Fetch data from all 14 pages
            for (let page = 1; page <= 14; page++) {
                const url = `http://0.0.0.0:8000/pharmacies?page=${page}`;
                const response = await axios.get(url);
                let pharm_data = response.data.pharmacies;

                for (let pharm in pharm_data) {
                    let pharmacy = pharm_data[pharm]
                    let city = pharmacy['city']
                    if (cities.get(city) === undefined) {
                        cities.set(city, 1)
                    } else {
                        cities.set(city, cities.get(city) + 1)
                    }                   
                }
            }

            // list of dictionaries
            const city_data = Array.from(cities, ([city, count]) => ({ city, count }));
            setData(city_data);
        };

        fetchData();
    }, []);

    return (
        <Container fluid>
          <Row className="justify-content-center mt-5">
            <h3 className="p-5 text-center">Number of Pharmacies Per City</h3>
          </Row>
          <Row className="justify-content-center">
            <Col xs={12} md={8}>
              <ResponsiveContainer width="100%" height={450}>
                <BarChart data={data} margin={{ bottom: 80 }}>
                  <XAxis dataKey="city" interval={0} angle={45} textAnchor="start" />
                  <YAxis />
                  <Tooltip />
                  <Bar dataKey="count" fill="#add8e6" />
                </BarChart>
              </ResponsiveContainer>
            </Col>
          </Row>
        </Container>
      );

    
}

export default PharmaciesPerCity
