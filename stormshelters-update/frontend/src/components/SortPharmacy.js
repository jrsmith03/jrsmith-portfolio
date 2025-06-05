import React, { useState } from 'react';
import '../css/sort.css'

function SortPharmacy({ onSortOptionChange }) {
  const [selectedOption, setSelectedOption] = useState(''); // Store the selected option
  const [selectedOrder, setSelectedOrder] = useState('asc'); // Default sorting order

  const handleOptionChange = (event) => {
    setSelectedOption(event.target.value);

    // Determine the sorting order based on the selected option
    const order = event.target.value.includes('Desc') ? 'desc' : 'asc';
    setSelectedOrder(order);

    // Call the parent component's callback with the selected option and order
    onSortOptionChange(event.target.value, order);
  };

  return (
    <div>
      <label>Sort By: </label>
      <select className='sort-dropdown' value={selectedOption} onChange={handleOptionChange}>
        <option value="None">Select</option>
        <option value="City Asc">City (Ascending)</option>
        <option value="City Desc">City (Descending)</option>
        <option value="Name Asc">Name (Ascending)</option>
        <option value="Name Desc">Name (Descending)</option>
        <option value="Distance Asc">Distance (Ascending)</option>
        <option value="Distance Desc">Distance (Descending)</option>
      </select>
    </div>
  );
}

export default SortPharmacy;
