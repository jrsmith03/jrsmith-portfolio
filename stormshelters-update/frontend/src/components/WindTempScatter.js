import React, { useEffect, useState } from "react";
import axios from "axios";
import Container from "react-bootstrap/Container";
import Row from 'react-bootstrap/Row';
import Col from 'react-bootstrap/Col';
import { ScatterChart, Scatter, XAxis, YAxis, ZAxis, Tooltip, LabelList, ResponsiveContainer } from 'recharts';

function getData() {
        // Fetch data from all 4 pages
        var values = [];
        console.log("test");
        var index = 0;
        for (let page = 1; page <= 4; page++) {
            console.log(page);
            var mainURL =`http://0.0.0.0:8000/cities?page=${page}`;
            var mainreq = new XMLHttpRequest();
            mainreq.open('GET', mainURL, false); 
            mainreq.send(null);
            // We get just the size from API
            var cities_data = JSON.parse(mainreq.responseText).cities;

            for (let city_id in cities_data) {
                console.log("index " + index)
                let city = cities_data[city_id];
                
                values[index++] = {name: city['name'], wind: city['wind_mph'], temp: city['temp_in_f']};
                
            }
        }
        console.log('city array', values); 
        return values;
}

const WindTempScatter = () => {
    var data = getData();
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