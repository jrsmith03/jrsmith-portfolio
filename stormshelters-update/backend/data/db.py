import mysql.connector
import json
import requests

# AWS RPS DB info
storm_host = "sql"
storm_user = "root"
storm_port = "3306"
storm_password = "StormShelters2023"
storm_database = "model"

print("attempting to connect")

# Database connection
stormshelters_db = mysql.connector.connect(
    host = storm_host,
    user = storm_user,
    password = storm_password,
    database = storm_database,
    port = storm_port
)

cursor = stormshelters_db.cursor()


def main():
    load_cities()
    load_pharmacies()
    load_shelters()
 
    # no shelter images necessary.

def load_cities() :
    # read json data from a file
    with open("cities_new.json", "r") as json_file:
        city_data = json.load(json_file)

    # Define the SQL query to insert data
    insert_query = """
    INSERT INTO cities (name, pop, temp_in_f, wind_mph, cond, precip_in)
    VALUES (%(name)s, %(pop)s, %(temp_in_f)s, %(wind_mph)s, %(cond)s, %(precip_in)s)
    """
    for entry in city_data:
        # deal with default values here instead of at db level
        if (entry['pop'] == '-') :
            entry['pop'] = '0'
        extracted_data = {
            "name": entry["name"],
            "pop": entry["pop"],
            "temp_in_f": entry["temp in f"],
            "wind_mph": entry["wind_mph"],
            "cond": entry["condition"],
            "precip_in": entry["precip_in"],
        }

        cursor.execute(insert_query, extracted_data)

    stormshelters_db.commit()
    print('City data inserted successfully')

def load_cities_images():
    get_cities_query = """SELECT name FROM cities"""

    cursor.execute(get_cities_query)
    city_names = cursor.fetchall()

    used_url = []
    list_pos = 0
    # iterate through city names, load each url
    for city_name in city_names:
        # google custom search, get image url
        print(city_name[0])
        pos = 0
        while True :
            image_url = fetch_image_url(city_name[0] + ' texas', pos)
            if not image_url in used_url : 
                list_pos = list_pos + 1
                used_url.insert(list_pos, image_url)
                break
            pos = pos + 1
               
        print(image_url)
        if image_url:
            # query and add url to sql
            real_name = city_name[0]
            print(real_name)
            cursor.execute(f"SELECT id FROM cities WHERE name = '{real_name}'")
            city_id = cursor.fetchall()
            print(city_id[0][0])
            load_image_query = f"""UPDATE cities SET image = '{image_url}' WHERE id = '{city_id[0][0]}'"""
            cursor.execute(load_image_query)
            stormshelters_db.commit()
        else:
            print("failed to get image url")
    stormshelters_db.commit()
    
    cursor.close()
    stormshelters_db.close()


def load_pharm_images():
    get_pharmacy_query = """SELECT name FROM pharmacies"""

    cursor.execute(get_pharmacy_query)
    pharmacy_names = cursor.fetchall()
    cvs_default = "https://1000logos.net/wp-content/uploads/2020/03/CVS-Pharmacy-Logo.png"
    walgreens_default = "https://www.walgreens.com/images/adaptive/si/1485908_WAG_Signature_logo_RGB_750x208.png"
    heb_default = "https://upload.wikimedia.org/wikipedia/en/thumb/a/a1/H-E-B_logo.svg/1280px-H-E-B_logo.svg.png"

    # iterate through city names, load each url
    print (pharmacy_names)
    id = 0
    for pharmacy_name in pharmacy_names:
        id = id + 1
        # google custom search, get image url
        print(pharmacy_name[0])
        image_url = fetch_image_url(pharmacy_name[0], 0)       
        print(image_url)
        if not image_url : 
            if pharmacy_name[0] == 'CVS Pharmacy' : 
                image_url = cvs_default
            if pharmacy_name[0] == 'Walgreens' : 
                image_url = walgreens_default
            if pharmacy_name[0] == 'H-E-B Pharmacy' :
                image_url = heb_default
    
        if image_url:
            # query and add url to sql
            real_name = pharmacy_name[0]
            print(real_name)
            # cursor.execute(f"SELECT id FROM pharmacies_new WHERE name = '{real_name}'")
            # pharm_id = cursor.fetchall()
            # print(pharm_id[0][0])
            load_image_query = f"""UPDATE pharmacies SET image = '{image_url}' WHERE id = '{id}'"""
            cursor.execute(load_image_query)
            stormshelters_db.commit()
            # print("hello!")
        else:
         

            print("failed to get image url")
    stormshelters_db.commit()
    
    cursor.close()


def load_pharmacies():
    # read json data from a file
    with open("medical_new.json", "r") as json_file:
        med_data = json.load(json_file)

    cursor = stormshelters_db.cursor()
    # Define the SQL query to insert data
    insert_query = """
    INSERT INTO pharmacies (name, city, address, distance_m, categories, longitude, latitude, website)
    VALUES (%(name)s, %(city)s, %(address)s, %(distance_m)s, "healthcare", %(longitude)s, %(latitude)s, %(website)s)
    """
    for entry in med_data:
        extracted_data = {
            "name": entry["name"],
            "city": entry["city"],
            "address": entry["address"],
            "distance_m": entry["distance_m"],
            "categories ": "healthcare",
            "longitude":entry["longitude"],
            "latitude":entry["latitude"],
            "website":entry["website"]
        }

        cursor.execute(insert_query, extracted_data)

    stormshelters_db.commit()
    print('Data inserted successfully') 


def load_shelters():
    # read json data from a file
    with open("food_new.json", "r") as json_file:
        food_data = json.load(json_file)
    with open("shelters_new.json", "r") as json_file:
        shelter_data = json.load(json_file)

    cursor = stormshelters_db.cursor()
    # Define the SQL query to insert data
    insert_query = """
    INSERT INTO shelters (name, url, is_closed, city, rating, display_address, display_phone, image_url, latitude, longitude)
    VALUES (%(name)s, %(url)s, %(is_closed)s, %(city)s, %(rating)s, %(display_address)s, %(display_phone)s, %(image_url)s, %(latitude)s, %(longitude)s)
    """

    for entry in food_data.get("businesses"):
        extracted_data = {
            "name": entry["name"],
            "url": entry["url"],
            "is_closed": entry["is_closed"],
            "rating": entry["rating"],
            "city": entry["location"]["city"],
            "display_address": ', '.join(entry["location"]["display_address"]),
            "display_phone": entry["display_phone"],
            "image_url": entry["image_url"],
            "latitude": entry['coordinates']['latitude'],
            "longitude": entry['coordinates']['longitude']
        }
        cursor.execute(insert_query, extracted_data)

    stormshelters_db.commit()

    for entry in shelter_data.get("businesses"):
        extracted_data = {
            "name": entry["name"],
            "url": entry["url"],
            "is_closed": entry["is_closed"],
            "rating": entry["rating"],
            "city": entry["location"]["city"],
            "display_address": ', '.join(entry["location"]["display_address"]),
            "display_phone": entry["display_phone"],
            "image_url": entry["image_url"],
            "latitude": entry['coordinates']['latitude'],
            "longitude": entry['coordinates']['longitude']
        }
        cursor.execute(insert_query, extracted_data)

    stormshelters_db.commit()
    print('Shelter inserted successfully') 

# Using Pexels API
# Written by Rohit, modified by John
def fetch_image_url(query, pos):
    print("query: " + query)
    api_key = "AIzaSyAcJPY_z5AiJAF0bngy4Ek6M0E3pTTGcTk"
    cse_id = "b4dcf4643af1e4b07"    # custom search engine ID

    url = f'https://www.googleapis.com/customsearch/v1?key={api_key}&cx={cse_id}&q={query}&searchType=image'

    # Gather http data from Pexels based on city name query
    response = requests.get(url)

    # 200 is code for success
    if response.status_code == 200:
        images_data = response.json()
        
        print(images_data["items"][pos]["link"])
        query_result = images_data["items"][pos]["link"]
        if query_result:
            return query_result
        return 0

    else:
        print("request failed with status code: ", response.status_code)



if __name__ == "__main__":
  main()


