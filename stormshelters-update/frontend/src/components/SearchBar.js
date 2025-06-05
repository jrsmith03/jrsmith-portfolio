import {withRouter} from 'react-router-dom'
import React, { Component, Redirect, useState } from 'react';

import '../css/Navbar.css';

const SearchBar = ({ model }) => {
  const [formInput, formValue] = useState('');

  const formInputChange = (inputVar) => {
    formValue(inputVar.target.value);
  };
  
  function backendSearch(event) {
    event.preventDefault(); 
    if (model == 'cities') model = 'city';
    var url = "/SearchDisplay/" + model + "/" + formInput;
    console.log("try to redirect to: " + url);
    // JRS - I did this because the React useHistory had some conflict with our site configuration.
    window.location.href= url;
  }

  return (
    <div class="input-group">
      <form onSubmit ={backendSearch}>
        <input type="search" id="form1" class="form-control1" value={formInput} onChange={formInputChange} placeholder="Search..." />
        <button className = "buttonSmall" type="submit">Search</button>
      </form>
  </div>

  );
  
}

export default SearchBar;
