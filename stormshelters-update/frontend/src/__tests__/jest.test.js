import { BrowserRouter } from 'react-router-dom';
import renderer from 'react-test-renderer';
import { render, screen, fireEvent, cleanup, waitFor } from '@testing-library/react';


import App from '../App.js'

import Navbar from '../components/Navbar.js';

import About from '../components/About.js';
import Home from '../components/Home.js';
import Cities from '../components/Cities.js';
import Pharmacies from '../components/Pharmacies.js';
import Shelters from '../components/Shelter.js';
// import CityCard from '../components/CityModel.js';

afterEach(() => {
  cleanup();
});


it('Init NavBar', () => {
    const component = renderer.create(
      <BrowserRouter>
        <Navbar />
      </BrowserRouter>,
    );
  
    let tree = component.toJSON();
    expect(tree).toMatchSnapshot();
  });
  

  it('Init About', () => {
    const component = renderer.create(<BrowserRouter><About /></BrowserRouter>);
  
    let tree = component.toJSON();
    expect(tree).toMatchSnapshot();
  });


  it('Init App', () => {
    const component = renderer.create(<App />);
  
    let tree = component.toJSON();
    expect(tree).toMatchSnapshot();
  });
  
  it('Init HomeCard', () => {
    const component = renderer.create(
      <BrowserRouter>
        <Home />
      </BrowserRouter>,
    );
  
    let tree = component.toJSON();
    expect(tree).toMatchSnapshot();
  });


  test('Home renders', async () => {
    render(<BrowserRouter><Home /></BrowserRouter>);
    await waitFor(() => {
      expect(screen.getByText(/Prepare for the Storms./)).toBeInTheDocument();
    });
  });
  
  test('About renders', async () => {
    render(<BrowserRouter><About /></BrowserRouter>);
    await waitFor(() => {
      expect(screen.getByText(/About Page/)).toBeInTheDocument();
    });
  });


  test('Leagues renders', async () => {
    render(<BrowserRouter><Cities /></BrowserRouter>);
    await waitFor(() => {
      expect(screen.getByText('Cities')).toBeInTheDocument();
    });
  });
  

  test('Shelters renders', async () => {
    render(<BrowserRouter><Shelters /></BrowserRouter>);
    await waitFor(() => {
      expect(screen.getByText('Shelters & Food pantries')).toBeInTheDocument();
    });
  });
  

  test('Pharmacies renders', async () => {
    render(<BrowserRouter><Pharmacies /></BrowserRouter>);
    await waitFor(() => {
      expect(screen.getByText('Pharmacies')).toBeInTheDocument();
    });
  });

  test('About has tools', async () => {
    render(<BrowserRouter><About /></BrowserRouter>);
    await waitFor(() => {
      expect(screen.getByText('Tools Used:')).toBeInTheDocument();
    });
  });
  
  