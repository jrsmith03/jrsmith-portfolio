import React from 'react';
import '../css/Navbar.css';

function Navbar() {
  return (
    <nav className="navbar">
      <ul className="nav-list">
      <li className="nav-item">
          <a href="/" className="nav-link">
            StormShelters
          </a>
        </li>
        <li className="nav-item">
          <a href="/about" className="nav-link">
            About
          </a>
        </li>
        <li className="nav-item">
          <a href="/cities" className="nav-link">
            Cities
          </a>
        </li>
        <li className="nav-item">
          <a href="/shelter" className="nav-link">
            Shelter & Relief
          </a>
        </li>
        <li className="nav-item">
          <a href="/pharmacies" className="nav-link">
            Pharmacies
          </a>
        </li>
        <li className="nav-item">
          <a href="/ourVisualizations" className="nav-link">
            Visualizations
          </a>
        </li>
        <li className="nav-item">
          <a href="/providerVisualizations" className="nav-link">
            Provider Visualizations
          </a>
        </li>
      </ul>
    </nav>
  );
}

export default Navbar;
