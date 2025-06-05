import app
import unittest

app.app.config["TESTING"] = True
client = app.app.test_client()

class Tests(unittest.TestCase):
    def test_cities_page(self):
        with client:
            response = client.get('/cities')
            self.assertEqual(response.status_code, 200)
            data = response.json["cities"]
            self.assertEqual(len(data), 9)

    def test_shelters_page(self):
        with client:
            response = client.get('/shelters')
            self.assertEqual(response.status_code, 200)
            data = response.json["shelters"]
            self.assertEqual(len(data), 9)

    def test_pharmacies_page(self):
        with client:
            response = client.get('/pharmacies')
            self.assertEqual(response.status_code, 200)
            data = response.json["pharmacies"]
            self.assertEqual(len(data), 9)

    def test_city(self):
        with client:
            response = client.get("/cities/6")
            self.assertEqual(response.status_code, 200)
            data = response.json["city"]
            self.assertEqual(data["name"], "Houston")

    def test_shelter(self):
        with client:
            response = client.get("/shelters/34")
            self.assertEqual(response.status_code, 200)
            data = response.json["shelter"]
            self.assertEqual(data["name"], "The Abundant Harvest")
            self.assertEqual(data["city"], "Spring")

    def test_pharmacy(self):
        with client:
            response = client.get("/pharmacies/121")
            self.assertEqual(response.status_code, 200)
            data = response.json["pharmacy"]
            self.assertEqual(data["name"], "CVS Pharmacy")
            self.assertEqual(data["city"], "Jacinto City")

    def test_search_1(self):
        with client:
            response = client.get("/search/city/cloudy")
            self.assertEqual(response.status_code, 200)
            data = response.json["data"]
            self.assertEqual(data[0]["cond"], "Partly cloudy")
    
    def test_search_2(self):
        with client:
            response = client.get("/search/all/houston")
            self.assertEqual(response.status_code, 200)
            city_data = response.json["cities"]
            self.assertEqual(city_data[0]["name"], "Houston")
            shelter_data = response.json["shelters"]
            self.assertEqual(shelter_data[0]["city"], "Houston")

    def test_sort_1(self):
        with client:
            response = client.get("/cities?sort=city&order=asc")
            data = response.json["cities"]
            self.assertEqual(data[0]["name"], "Aldine")

    def test_sort_2(self):
        with client:
            response = client.get("/shelters?sort=rating&order=desc")
            data = response.json["shelters"]
            self.assertEqual(data[0]["rating"], 5.0)
    
    def test_filter_1(self):
        with client:
            response = client.get("/pharmacies?city=Houston")
            data = response.json["pharmacies"]
            self.assertEqual(data[0]["name"], "CVS Pharmacy")

    def test_filter_2(self):
        with client:
            response = client.get("/shelters?rating=1-2")
            data = response.json["shelters"]
            self.assertEqual(data[0]["name"], "Star of Hope Cornerstone Community")


if __name__ == "__main__":
    unittest.main()