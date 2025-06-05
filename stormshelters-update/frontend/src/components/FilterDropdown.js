import React, { useState } from "react";
import "../css/filter.css";

const FilterDropdown = (props) => {
  const { title, items, scroll, onChange } = props;
  const [showOptions, setShowOptions] = useState(false);
  const [choice, setChoice] = useState(title);

  const handleClick = (value) => {
    setChoice(value);
    onChange(value);
    setShowOptions(false);
  };

  return (
    <div className="filter-container" onClick={() => setShowOptions(!showOptions)}>
      {choice}
      {showOptions && (
        <div className="filter-content" style={scroll ? { maxHeight: "20rem" } : {}}>
          {items.map((item, index) => (
            <div key={index} onClick={() => handleClick(item)}>
              {item}
            </div>
          ))}
        </div>
      )}
    </div>
  );
};

export default FilterDropdown;
