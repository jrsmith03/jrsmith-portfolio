import React, { useEffect, useState } from "react";
import axios from "axios";
import Container from "react-bootstrap/Container";
import Row from 'react-bootstrap/Row';
import Col from 'react-bootstrap/Col';
import { PieChart, Pie, ResponsiveContainer, Cell, Tooltip, Legend, Label} from 'recharts';

const ShelterRatings = () => {
    // const [data, setData] = useState(null)

    // useEffect(() => {
    //     const fetchData = async () => {
    //         const ratings = new Map()

    //         for (let page = 1; page <= 4; page++) {
    //             const url = `http://0.0.0.0:8000/shelters?page=${page}`
    //             const response = await axios.get(url);
    //             let shelter_data = response.data.shelters;

    //             for (let sh in shelter_data) {
    //                 let shelter = shelter_data[sh]
    //                 let rating = shelter['rating']
    //                 if (ratings.get(rating) === undefined) {
    //                     ratings.set(rating, 1) 
    //                 } else {
    //                     ratings.set(rating, ratings.get(rating) + 1)
    //                 }
    //             }
    //         }
    //         // list of dictionaries
    //         const pie_data = Array.from(ratings, ([rating, count])  => ({ rating, count }));
    //         setData(pie_data)
    //         console.log("data", pie_data)
    //     };
    //     fetchData();

    // }, [])

    const data = [
        
        {rating: 3, count: 4},
        
        {rating: 2, count: 2},
       
        {rating: 5, count: 8},
       
        {rating: 3.5, count: 5},
     
        {rating: 1.5, count: 1},
       
        {rating: 2.5, count: 4},
     
        {rating: 4, count: 5},
      
        {rating: 1, count: 1},
       
        {rating: 4.5, count: 6}]
    const COLORS = ["#0088FE", "#00C49F", "#483d8b", "#d8bfd8", "#6495ed", "#00bfff", "#add8e6", "#b0c4de", "#b0e0e6"];
    console.log("data after", data)
        return (
           

<Container fluid>
            <Row className="justify-content-center mt-5">
                <h3 className="p-5 text-center">Shelter Ratings Count</h3>
            </Row>
            <Row className="justify-content-center">
                <Col xs={12} md={8}>
                    <ResponsiveContainer width="100%" height={400}>
                        <PieChart width={800} height={600}>
                            <Pie
                                data={data}
                                dataKey="count"
                                nameKey="rating"
                                cx="50%"
                                cy="50%"
                                outerRadius={150}
                                fill="#8884d8"
                                label={(entry) => entry.count > 0 ? entry.rating : ''}
                            >
                                {data.map((entry, index) => (
                                    <Cell
                                        key={`cell-${index}`}
                                        fill={COLORS[index % COLORS.length]}
                                    />
                                ))}
                            </Pie>
                            <Tooltip />
                            <Legend />
                            <Label value="Count" position="centerBottom" />
                        </PieChart>
                    </ResponsiveContainer>
                </Col>
            </Row>
        </Container>
    );


        
}

export default ShelterRatings;