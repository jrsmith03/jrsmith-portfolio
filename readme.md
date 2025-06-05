# John R. Smith

- Phone: (956)-426-8222
- Email: johnrsmith2003@gmail.com
- LinkedIn: https://www.linkedin.com/in/john-smith-b38263327/ 
# Project Portfolio

This repository contains my biggest and most notable projects. My biggest passion is in low level systems development in C and C++. I have also worked extensively in Python for the backend parts of my full-stack web app and for ML (in the Matplotlib, scikit-learn, and numpy). I've made great use of C++ for Computer Graphics and Game Programming. I've used JavaScript (ReactJS) for frontend development.

Please use this README to explore links to the most relevant code in the projects along with descriptions of my biggest accomplishments.

# [Command and Control](\c2)
- Gained privileged remote access to a vulnerable Linux server by locating password through packet analysis and
exploitation of strace command and insecure CRON job.
- Built an obscured remote shell server ([backdoor](c2/backdoor.c)) and client [utility](c2/c2_client.c) with encrypted traffic and fullly-privileged
access. Used sockets to issue commands to a listening port and publish output to the user, fork/exec for process
management, and the tiny-aes library for end-to-end encryption.
- Obscured the backdoor via shell scripts that replaced an instance of the ssh program with the backdoor upon boot and also replaced directory listing and [process listing](c2/ls_wrapper.sh) utilities with scripts that obscured the backdoor process. Further leveraged the CRON job vulnerabilities to allow the script to execute.

# [Full Stack Web App](\stormshelters-update)
- Collaborated in a group of five to develop a full-stack web application that dynamically displayed relevant information
from various APIs.
- Used asynchronous methods to query our backend database to fetch data for the frontend (see [here](./stormshelters-update/frontend/src/components/Cities.js)). Used hooks to dynamically update content based on filtering/pagination changes. Displayed information [cards](./stormshelters-update/frontend/src/components/CityModel.js) via a React object to allow for easy reusability of styling/interface logic components across the site.
- Implemented a [search interface](./stormshelters-update/frontend/src/components/SearchDisplay.js) in the frontend, with a reusable and expandable display that could combine results across all endpoints. Also implemented highlighting algorithm for search query by doing a fuzzy character match across all returned text.
- Also contributed to database interface and [design](stormshelters-update/backend/ddl.sql), configuring MySQL in AWS, writing [Python scripts](stormshelters-update/backend/data/scrape_data.py) to query
source APIs, and writing website architecture documentation.
- Worked inside our AWS stack, utilizing Docker to run containers of our front/back ends locally and later deploy to the cloud. Later, I individually built a database container to allow that component to also run locally. 
- Gained experience working with CI/CD pipelines in Gitlab to provide testing (both frontend via Selenium and unit tests for backend API)

# [Traffic Accident Prediction](TrafficPrediction/proj.ipynb)
* Found a governemnt dataset which details the conditions (~25 features)of a about 150,000 traffic accidents. Used numpy and Matplotlib to analyze the dataset and perform data cleaning such as filling in missing values, dropping irrevelant features, and combining features together to derive more meaninful patterns. 
* Used scikit-learn to perform classification-based ML.
    * Created pipelines to perform appropriate dimensionality reduction and one-hot encoding of categorical features. 
    * Also used numpy to hold out testing/training sets when appropriate to avoid data leakage.
    * Used n-fold cross-validation loops to find optimal hyperparamaters for Neural Network, SVM and ensemble models.
    * Used the Decision Tree, KNN, SVM, Neural Network, and Random Forest algorithms to derive a prediction of severity (1 through 4).
    * Produced a confusion matrix to analyze the accuracy and recall of each method.
    * Used both numpy and a SMOTE library to deal with a label imbalance in the data. First tried dropping instances of the majority (80% of instances) lablel, and then performed the oversampling of the minority label which produced multiple new instances with similar properties to their nearest neighbors. The later method improved recall by 20%.
# [Media Sorting Application](media-sort)
- Utility to index, analyze and relocate media files in a directory. 
- First, [recursively traversed](media-sort/process-dir.c) every file within a directory. Analyzed each file to see if it was media, and then used file system API to derive metadata from it. 
- [Created directories](media-sort/process-file.c) based on user specifications, and moved/renamed media files based on the metadata (i.e. date taken). 
- Kept a custom log format for all file changes to ensure durability given application failure. 
- Given the unreliability of file system dates, wrote a tool that analyzed [EXIF encodings](media-sort/exif-util.c) in JPG files to extract relevant data to develop a heuristic for determining date and
relevancy.

# [Procedurally-Generated Terrain Demo](sasaki-project-clone)
* ### Also includes multiplayer [networking](sasaki-project-clone/src/net) (lobby support), custom [AI system](sasaki-project-clone/src/ai) that allows for JSON specification of agent behaviors, and an intuitive player package with custom physics behaviors.
* Generated [terrain meshes](sasaki-project-clone/src/platform/ProceduralPlane.cpp) based on Perlin-noise function. Sampled the function's Y coordinate at every point to see how to perturb the mesh. Used Godot's SurfaceTool to coordinate and coloring/texturing data at each point. Used this same hightmap to specify mob/object generation at certain depths
* Implemented a [chunking system](sasaki-project-clone/src/platform/Level.cpp) for performance. This allowed smaller pieces (within a customizable view distance) to be generated. Intra-biome chunks were continuous as they shared the same noise function.
* Implemented a [biome system](sasaki-project-clone/src/platform/BiomeSpec.cpp) that randomally selected a different generation specification (i.e. color schemes, types of mobs, terrain varity/amplitude/physical charactistics). Ensured smooth transitions by performing linear blending. 
* Further improved performance via a multithreaded chunk [pre-generation system](sasaki-project-clone/src/platform/Level.cpp), where idle cycles would contribute towards pre building arrays specifying the chunk coordinates and performing blending. Kept a map structure of all biomes to provide continuity context and metadata about loading progress.
* Please view my *extensive*  [technical documentation](https://docs.google.com/document/d/1h38XRwVsb6sXBtY6vp662-lQxCIrpp6viZHCWk3HzNc/edit?tab=t.0#heading=h.fw5b6vvd0di9) outlining the algorithms and system design for the project.
* I also wrote many more technical docs throughout this project and my Graphics class. Here's a few:
    * [AI system](https://docs.google.com/document/d/1zXz3D65diKs2vCYJ6tYEYdY2nsZke57dQzGm1EEXFtI/edit?tab=t.0#heading=h.ybo076yl3qtn)
    * [Raytracer Application](https://docs.google.com/document/d/1gVWFMx0-x3MVRb6G71p3sDGOjdMWLjpOg7Gnt_zydsc/edit?tab=t.0)
    * [Traffic Simulation Plugin](https://docs.google.com/document/d/1O8mpTvQJYsX2rGy8UXILxIn6hxQ4ol9m_ayMS1qTCbU/edit?tab=t.0#heading=h.558rgclapg7u)
# [Energy-Aware Task Scheduling in Operating Systems](task-scheduling.pdf) 
* Literature review investigating the performance and energy utilization effects on various informed task
scheduling algorithms.
*  Detailed parallels in frequency-scaling and static/dynamic process analysis across homogeneous/heterogeneous
architectures. 

## About Me
![image](picture.png)
I graduated with a Bachelors of Science in Computer Science degree from [The University of Texas at Austin ](https://www.cs.utexas.edu/news/2024/ut-computer-science-undergraduate-program-ranks-among-best)(ranked #10 in US) in May 2025, and I will be perusing a Masters of Science in CS [from UT ](https://www.cs.utexas.edu/news/2025/us-news-ranks-ut-austin-computer-science-among-best-graduate-program-rankings)(ranked #7 in US) in the fall. 

I grew up in McAllen, Texas and went to school at South Texas ISD [World Scholars](https://worldscholars.stisd.net/), where I graduated as class Valedictorian and earned an International Baccalaureate diploma. This program allowed me to conduct extensive interdisiplinary research in Psychology, History, Politics, and Theory of Knowledge. I served as an officer of my Technology Student Association chapter for all four years; as President, I introduced programs to help students explore competitions, find team members, and check for competition readyness. I competed in web design and parliamentary procedure, reaching Nationals my senior year. I significantly honed my public speaking and leadership skills in TSA. 

At UT, I was a member of the IEEE Computer Science society and a voting outreach organization for my first year. My junior year, I joined a Battle Bots team where I selected and installed electrical components into a 3lb robot. Academically, my biggest accomplishment was building major components of an educational operating system (Pintos) in my Operating Systems class. I also built components of an ARM system emulator, which included a 5 stage simulated pipeline. I led my team for the [full-stack web application](stormshelters-update) as well.

My biggest interests are in Machine Learning and Systems Programming. In my ML class, we explored the Scikit-learn library, pandas, and numpy to train various classification models to identify traffic crash severity. I perused a Business minor, which has helped refine my understand of management/leadership strategies, product marketing, and management information systems. Sparked during my time as an IB student, I also have a casual interest in human psychology.

Outside of programming, I run a technology [YouTube channel](https://www.youtube.com/@MINDtek1) that has amassed almost 1 million combined views. My most popular videos provide [troubleshooting guides](https://www.youtube.com/watch?v=7zFP-wvkJHk) for software, [setup guides](https://www.youtube.com/watch?v=kUYMYJOAF1c), and hardware [reviews](https://www.youtube.com/watch?v=JwV5x3MhgVk&t=55s).

Feel free to review my [resume](Resume2025.pdf) and email me with any business inqueries. I am open to employment and freelance/contract work.