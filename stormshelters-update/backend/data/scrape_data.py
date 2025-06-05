import json
import requests

# Use the api ninja Cities API to grab a list 
# Currently bound the city search to Texas

def main() :
    get_texas_cities()
    scrape_shelter_data()
    scrape_med_data()

cities_in_texas = []
coord_list = []


def get_texas_cities():
    weather_url = "http://api.weatherapi.com/v1/forecast.json"
    weather_key = '744d573dcf37484986351000230510'
    city_url = 'http://api.geonames.org/searchJSON?country=US&adminCode1=TX&featureClass=P&minPopulation=500&maxRows=500&username=jrsmith03'
    city_response = requests.get(city_url)
    city_data = city_response.json()
    print(city_data)
    for city in city_data['geonames'] :
        print("name:", city['toponymName'], " population: ", city['population'])
        params = {
            'key': weather_key,
            'q': city['toponymName']
        }
        cities_in_texas.append(city['toponymName'])
        coord_list.append([city['lng'], city['lat']])
        # weather api get request
        forecast_response = requests.get(weather_url, params=params)
       
        forecast_data = forecast_response.json()
        if (forecast_response.status_code == 200) :
            weather_data = {"name": city['toponymName'], "pop":city['population'], "temp in f": forecast_data['current']['temp_f'], "wind_mph":forecast_data['current']['wind_mph'], "condition":forecast_data['current']['condition']['text'], "precip_in": forecast_data['current']['precip_in']}
            create_response.append(weather_data)
        else :
            print("Something went wrong with Weather API.")
    # write json output to file
    file = open("cities_new.json", "w")
    json.dump(create_response, file, indent=2)
    file.write('\n')
    file.close()


create_response = []


# scrape shelter data    
def scrape_shelter_data():
    yelp_api_key = "ZwMb0UM46E2Fjr2075F5B9whbN8M2D9kN6kyg_UI7ckI6e7nhflpAxV7zdjBGu5yXItPwZlcdeReIuY2T96IKE_XlNRfx5UTHYA4dMDCxw0KfLMIx7Dl2RSeyekdZXYx"
    # yelp_client_id = "6aT9O-M3-H7ucEIOQsUt5w"

    url = "https://api.yelp.com/v3/businesses/search"

    headers={
        'Authorization': 'bearer ' + yelp_api_key
    }

    for city in cities_in_texas:
        shelter_params = {
            'term': 'homeless shelter',
            'location': city
        }

        food_pantry_params = {
            'term': 'food pantry',
            'location': city
        }

        shelter_response = requests.get(url, params=shelter_params, headers=headers)
        if (shelter_response.status_code == 200) :
            shelter_data = shelter_response.json()
            file = open("shelters_new.json", "w")
            json.dump(shelter_data, file, indent=2)
            file.write('\n')
            file.close()

            food_response = requests.get(url, params=food_pantry_params, headers=headers)
            if (food_response.status_code == 200) :
                food_data = food_response.json()
                file = open("food_new.json", "w")
                json.dump(food_data, file, indent=2)
                file.write('\n')
                file.close()
            else : 
                print("Unable to query Food api.")
        else:
            print("Unable to query Shelter api.")

# scrape natural disaster data
def scrape_med_data():
    create_med_response = []
    geo_place_url = 'https://api.geoapify.com/v2/places'

    for coord in coord_list:
        med_params = {
            'apiKey': '8e30cd9a70c440b59c2e1c7e486db8b1',
            'categories': 'healthcare.pharmacy',
            'bias': f'proximity:{coord[0]},{coord[1]}',
            'limit': 5
        }

        hospital_response = requests.get(geo_place_url, params=med_params)
        if (hospital_response.status_code == 200) :
            hospital_data = hospital_response.json()

            features = hospital_data["features"]
            for f in features:
                prop = f.get("properties")
                f_data = {"name": prop.get("name"), "city": prop.get("city"), "address": prop.get("formatted"), "cat": prop.get("categories"), "distance_m": prop.get("distance"), "website":prop.get("website"), "suburb": prop.get("suburb"), "latitude":prop["lat"], "longitude": prop["lon"]}
                create_med_response.append(f_data)
        else :
            print("Error getting data from GeoAPI, please try again.")
    file = open("medical_new.json", "w")
    json.dump(create_med_response, file, indent=2)
    file.write('\n')    
    file.close()

scrape_med_data()



if __name__ == "__main__":
  main()
