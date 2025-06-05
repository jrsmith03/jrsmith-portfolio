/* 
    MetroPulse's visualizations using D3.
    **all need to be a different type!
    1. 
    2.
    3.
    
*/
import Container from "react-bootstrap/Container";
import Row from 'react-bootstrap/Row';
import Col from 'react-bootstrap/Col';
import '../css/Visualization.css';

import { PieChart, Pie, Cell, BarChart, Bar, XAxis, YAxis, Tooltip, ResponsiveContainer, CartesianGrid, Legend, Line, LineChart, Label } from 'recharts';

// https://backend.metropulse.link/centers?borough=bronx

function getCenters() {
    
    var boroughs = new Array('Bronx', 'Brooklyn', 'Manhattan', 'Queens');
    var values = [];
    for (var borough in boroughs) {
        var mainURL = `https://backend.metropulse.link/centers?borough=${boroughs[borough]}`;
        var mainreq = new XMLHttpRequest();
        mainreq.open('GET', mainURL, false); 
        mainreq.send(null);
        // We get just the size from API
        var number = JSON.parse(mainreq.responseText).total_size;
        // Prepare the data in a fortmat that Recharts will like
        var data = { name: boroughs[borough], Centers: number };
        values[borough] = data;
    }
    return values;
}

function getFacilities() {
    var boroughs = new Array('Bronx', 'Brooklyn', 'Manhattan', 'Queens');
    var values = [];
    for (var borough in boroughs) {
        var mainURL = `https://backend.metropulse.link/hospitals?borough=${boroughs[borough]}`;
        console.log(mainURL);
        var mainreq = new XMLHttpRequest();
        mainreq.open('GET', mainURL, false); 
        mainreq.send(null);
        // We get just the size from API
        var number = JSON.parse(mainreq.responseText).total_size;
        // Prepare the data in a fortmat that Recharts will like
        var data = { name: boroughs[borough], Centers: number };
        values[borough] = data;
    }
    console.log(values);
    return values;
}

function getPops() {
    var values = [
        {
            Range: "< 20,000",
            Number: 0
        },
        {
            Range: "20,000-30,000",
            Number: 0
        },
        {
            Range: "30,000-40,000",
            Number: 0
        },
        {
            Range: "40,000-50,000",
            Number: 0
        },
        {
            Range: "> 50,000",
            Number: 0
        }, 
        
    ];
    var mainURL = `https://backend.metropulse.link/neighborhoods`;
    var mainreq = new XMLHttpRequest();
    mainreq.open('GET', mainURL, false); 
    mainreq.send(null);
    var json = JSON.parse(mainreq.responseText);
    console.log(json);
    for (var neighboorhood in json.data) {
        var popToAdd = json.data[neighboorhood].population;
        if (popToAdd < 20000) {
            values[0].Number++;
        } else if (popToAdd > 20000 && popToAdd < 30000) {
            values[1].Number++;
        } else if (popToAdd > 30000 && popToAdd < 40000) {
            values[2].Number++;
        } else if (popToAdd > 40000 && popToAdd < 50000) {
            values[3].Number++;
        } else if (popToAdd > 50000 ) {
            values[4].Number++;
        }
    }
    return values;
}


function ProviderVis() {
    var popRanges = getPops();
    console.log(popRanges[1].Number)
    var centersPerBorough = getCenters();
    var facilitiesPerBorough = getFacilities();
    const COLORS = ["#0088FE", "#00C49F", "#483d8b", "#d8bfd8", "#6495ed", "#00bfff", "#add8e6", "#b0c4de", "#b0e0e6"];
    // console.log("mydata: " + myvals);

    return (
        // Display for Number of Testing Centers Per Borough
        <Container>
            <h1 className="text-center">MetroPulse Visualizations</h1>

            <h2 className="text-center">Number of Testing Centers Per Borough</h2>

            <ResponsiveContainer width = "80%" height={450} className="center">
                <BarChart width = {580} height={360} data={centersPerBorough}>
                    <CartesianGrid strokeDasharray="1 1" />
                    <XAxis dataKey="name"></XAxis>
                    <YAxis />
                    <Tooltip />
                    <Legend />
                    <Bar dataKey="Centers" fill="#515487" width="20%"/>
                </BarChart>
            </ResponsiveContainer>

            <h2 className="text-center">Number of Neighborhoods By Population</h2>

            <ResponsiveContainer width = "80%" height={450} className="center">
                <LineChart width = {580} height={360} data={popRanges}>
                    <CartesianGrid strokeDasharray="1 1" />
                    <XAxis dataKey="Range"></XAxis>
                    <YAxis />
                    <Tooltip />
                    <Legend />
                    <Line type="monotone" dataKey="Number" fill="#515487" width="20%"/>
                </LineChart>
            </ResponsiveContainer>

            <h2 className="text-center">Medical Facililtes by Borough</h2>

            <ResponsiveContainer width = "100%" height={450}>
                <PieChart width = {800} height={600}>
                    <Pie
                        dataKey="Centers"
                        nameKey="name"
                        data={facilitiesPerBorough}
                        cx="50%"
                        cy="50%"
                        outerRadius={150}
                        fill="#8884d8"
                    >
                    {facilitiesPerBorough.map((entry, index) => (
                                    <Cell
                                        key={`cell-${index}`}
                                        fill={COLORS[index % COLORS.length]}
                                    />
                                ))}
                    </Pie>
                    <Tooltip />
                    <Legend />
                    
                        
                </PieChart>
            </ResponsiveContainer>

        </Container>

    );
}

export default ProviderVis;
