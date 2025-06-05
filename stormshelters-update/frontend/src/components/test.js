import React, { useEffect, useState } from "react";
import axios from "axios";
import Container from "react-bootstrap/Container";
import Row from 'react-bootstrap/Row';
import Col from 'react-bootstrap/Col';
import { ScatterChart, Scatter, XAxis, YAxis, ZAxis, Tooltip, LabelList, ResponsiveContainer } from 'recharts';

const WindTempScatter = () => {
    const [data, setData] = useState(null);
    
    useEffect(() => {
        const fetchData = async () => {
            const wind_temp = new Map();

            // Fetch data from all 4 pages
            var values = [];
            console.log("test");
            var index = 0;
            for (let page = 1; page <= 4; page++) {
                console.log(page);
                const url = `http://0.0.0.0:8000/cities?page=${page}`;
                // const url = `http://0.0.0.0:8000/cities`;

                const response = await axios.get(url);

                let cities_data = response.data.cities;

                for (let city_id in cities_data) {
                    console.log("index " + index)
                    let city = cities_data[city_id]
                    
                    values[index++] = {name: city['name'], wind: city['wind_mph'], temp: city['temp_in_f']}
                    
                }
            }
            setData(values);
            console.log('city array', values);
        };

        fetchData();
    }, []);


    console.log("data: " + data);
    return (
        <Container fluid>
          <Row className="justify-content-center mt-5">
            <h3 className="p-5 text-center">Wind and Temperature For Each City</h3>
          </Row>
          <Row className="justify-content-center">
            <Col xs={12} md={8}>
              <ResponsiveContainer width="100%" height={450}>
                <ScatterChart  margin={{ bottom: 80 }}>
                  <XAxis dataKey="wind" type="number" name="Wind (mph)" />
                  <YAxis dataKey="temp" type="number" name="Temperature (°F)" domain={[75, 90]}/>
                  <ZAxis dataKey="name" type="text" name="City" />
                  <Tooltip cursor={{ strokeDasharray: '3 3' }} />
                  <Scatter
                    data={data}
                    fill="#8884d8"
                    shape="circle"
                  />
                
                </ScatterChart>
              </ResponsiveContainer>
            </Col>
          </Row>
        </Container>
      );

    
}

export default WindTempScatter