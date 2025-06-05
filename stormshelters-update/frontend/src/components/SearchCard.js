import React from 'react';
import { Link } from 'react-router-dom';
import '../css/Shelter.css';
import '../css/Search.css';

/* Author: John Smith */

/* Highlighting Logic: walk through N characters of query inside of string.
   If there is a match, add these characters to a Set of HTML elements that contain
   a span modifying the styling to add a highlight.
   If not, add them to a list of regularly-styled text.
   Display the result as the body of the search card. */
function highlight(str, query) {
  var idx = 0;
  var highlightCharElem = new Set();
  while (str[idx]) {
    // Check if the next N characters are the query
    var found = false;
    var lowerstr = str.toLowerCase();
    var tempH = new Array();
    var tempN = new Array();

    for (var i = 0; i < query.length; i++) {
      if (query.indexOf(lowerstr[i + idx], 0) !== -1) {
        tempH.push(<span className='highlight'>{str[i + idx]}</span>);
        tempN.push(<span key = {i + idx}>{str[i + idx]}</span>);

      } else {
          tempN.push(<span key = {i + idx}>{str[i + idx]}</span>);
        break;
      }
    }
    if (tempH.length === query.length) {
      highlightCharElem.add(tempH);
      idx += query.length;
    } else {
      highlightCharElem.add(tempN);
      idx += tempN.length;
    }
  }
  return highlightCharElem;
}
const SearchCard = ({ model, data, query }) => {
    var image = data.image_url || data.image || data.url;
    return (
      <Link style={{textDecoration: 'none'}} to ={`/cities/${data.id}`}>
      <div className='shelter-card'>

        <div className="shelter-image-container">
          <img src={image} className="shelter-card-img-top" />
        </div>
        <div className="shelter-card-body">
          <h2 className='shelter-card-title'>{highlight(data.name, query)}</h2>
        <ul className="list-group list-group-flush">
          {Object.entries(data).map(([key, val], index) => {
            // Filter the results internally. We obtain all of the data from the database. 
            if (key !== "url" && key !== "image_url" && key !== "image" && key !== "distance_m" && key !== "percip_in" && key !== "is_closed") {
              key = key.split('_').join(' ');
              key = key.charAt(0).toUpperCase() + key.slice(1);
              var highlightKey = highlight(key, query);
              var highlightVal = highlight(val.toString(), query);
              return (
                <li className='list-group-item' key={index}>
                  {highlightKey}: {highlightVal}
                </li>
              )
            } else {
              return null;
            }
          })}
        
        </ul>
      </div>
      </div>

      </Link>

    );
  };

export default SearchCard;
