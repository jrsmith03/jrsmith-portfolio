import React, { useEffect, useState } from 'react';
import '../css/About.css';

function About() {
  const [contributorStats, setContributorStats] = useState([]);
  const [totalCommits, setTotalCommits] = useState(0);
  const [totalIssues, setTotalIssues] = useState(0);

  useEffect(() => {
    const projectId = '50438097';
    const contributorData = [
      { name: 'aaronk2711', displayName: 'Aaron Kulkarni', name3: 'aaron-kulkarni', description: 'I am a CS major currently in my third year from New Jersey. My favorite activities outside of the classroom are basketball, judo, and guitar. Major Responsibilities: Backend', unittests:0},
      { name: 'shreya.n', displayName: 'Shreya Nakka', name3: '', description: 'I am a Computer Science major in my Junior year. In my free time, I enjoy playing tennis, volleyball, and painting. Major Responsibilities: Backend', unittests:6},
      { name: 'rohitc28', displayName: 'Rohit Chawla', name3: '', description: 'I am a Computer Science major in my Junior year. I have lived in Austin my entire life and I enjoy playing board games. Major Responsibilities: Frontend', unittests:0},
      { name: 'jrsmith0', displayName: 'John Smith', name3: '', description: 'I am a junior computer science major from the Rio Grande Valley. In my free time, I enjoy camping and spending time with family. Major Responsibilities: Frontend', unittests:0},
      { name: 'areye2020', displayName: 'Adriana Reyes', name3: '', description: 'I am a senior computer science major from Austin. Outside of CS I like to play guitar, skateboard, and draw. Major responsibilities: Frontend', unittests:0},
    ];

    async function fetchData() {
      let totalCommitsCount = 0;
      let totalIssuesCount = 0;

      // fetch issues
      const issuesResponse = await fetch(`https://gitlab.com/api/v4/projects/${projectId}/issues?per_page=100`);
      const issuesData = await issuesResponse.json();

      for (const contributor of contributorData) {
        let { name, displayName, name3, description } = contributor;

        // count issues
        if (name === 'jrsmith0') {
          name = 'johnrsmith2003';
        }

        let issuesCount = 0;
        for (const issue of issuesData) {
          if (issue.author.username === name) {
            issuesCount++;
          }
        }

        if (name === 'johnrsmith2003') {
          name = 'jrsmith0';
        }

        // fetch + count commits
        let commitsCount = 0;
        for (let pageNumber = 1; pageNumber <= 4; pageNumber++) {
          const commitsResponse = await fetch(`https://gitlab.com/api/v4/projects/${projectId}/repository/commits?author_username=${name}&per_page=100&page=${pageNumber}`);
          const commitsData = await commitsResponse.json();
          
          for (const commit of commitsData) {
            if (commit.author_name === displayName || commit.author_name === name || commit.author_name === name3) {
              commitsCount++;
            }
          }
        }

        totalCommitsCount += commitsCount;
        totalIssuesCount += issuesCount;

        setContributorStats((prevData) => [
          ...prevData,
          {
            name: name,
            displayName: displayName,
            issues: issuesCount,
            commits: commitsCount,
            description: description,
            unittests: contributor.unittests
          },
        ]);
      }

      setTotalCommits(totalCommitsCount);
      setTotalIssues(totalIssuesCount);
    }

    fetchData();
  }, []);

  return (
    <div className="about-container">
      <h1>About Page</h1>
      <p>
        StormShelters is an application that can provide those in Harris County
        that have been affected by natural disasters with resources for help and
        shelter. The website provides information on the different cities in the
        county, the disasters they were struck by, and different shelter locations
        nearby. Data sources used: Yelp, FEMA, Wikipedia, Census. Tools used: GitLab
        repository with CI/CD, Visual Studio Code, JavaScript, HTML/CSS, Postman,
        Bootstrap Through our research, we have found interesting correlations
        between data we didn't realize was so closely connected. The most
        interesting realization was how shelters were concentrated near the areas
        where many storms take place.
      </p>
      <div className="team-members-container">
        {contributorStats.map((contributor, index) => (
          <div className="about-card" key={index}>
            <img src={`../img/${contributor.name}.jpg`} alt={`contributor ${index + 1}`} />
            <div className="text">{contributor.displayName}</div>
            <div className="text">{`${contributor.description}`}</div>
            <div className="text">{`Number of issues: ${contributor.issues}`}</div>
            <div className="text">{`Number of commits: ${contributor.commits}`}</div>
            <div className="text">{`Number of unit tests: ${contributor.unittests}`}</div>
          </div>
        ))}
      </div>
      <div className="statistics">
        <p>{`Total commits: ${totalCommits}`}</p>
        <p>{`Total issues: ${totalIssues}`}</p>
      </div>
      <div className="center-text">
        <br />
        <a href="https://documenter.getpostman.com/view/29974721/2s9YR6aDwG">API Documentation</a>
        <br />
        <a href="https://gitlab.com/cs373-group5/stormshelters">GitLab Repository</a>
      </div>
      <div className="text-left">
        <br />
        <h2>Tools Used:</h2>
        <ul>
          <li>VS Code: Integrated Development Environment</li>
          <li>Zoom: Remote collaboration</li>
          <li>Namecheap: Domain name registration</li>
          <li>Route 53: Domain name resolution</li>
          <li>AWS Amplify: Frontend server hosting</li>
          <li>AWS EC2: Backend server hosting</li>
          <li>AWS RDS: Database hosting</li>
          <li>Postman: API Documentation</li>
          <li>Selenium/Jest: Testing Frameworks</li>
        </ul>

        <h2>Data sources:</h2>
        <ul>
          <li><a href="http://api.weatherapi.com/v1/forecast.json">Weather API</a></li>
          <li><a href="https://fusion.yelp.com">Yelp Fusion</a></li>
          <li><a href="https://developers.google.com/maps/documentation/geocoding/overview">GeoCoding API</a></li>
          <li><a href="https://www.geoapify.com/geocoding-api">GeoApify</a></li>
        </ul>
        <p>Each of these sites provides a RESTful API. For Phase 1, we first analyzed the documentation to find the appropriate endpoints. Then, we scraped the data using a curl request and filtered the data by hand to find the appropriate information. Finally, we added the data to a JSON file so that it could be re-used across different pages.</p>
        <br />
      </div>
    </div>
  );
}

export default About;