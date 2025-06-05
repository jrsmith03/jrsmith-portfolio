import unittest
from selenium import webdriver
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.support.select import Select


url = 'https://dev.stormshelters.me/'

class Test(unittest.TestCase):
    def setUp(self):
        chrome_options = Options()
        chrome_options.add_argument("--window-size=1920,1080")
        chrome_options.add_argument("--start-maximized")
        chrome_options.add_argument("disable-infobars")
        chrome_options.add_argument("--disable-extensions")
        chrome_options.add_argument("--disable-gpu")
        chrome_options.add_argument("--disable-dev-shm-usage")
        chrome_options.add_argument("--no-sandbox")
        chrome_options.add_argument("--headless")
        self.driver = webdriver.Chrome(options=chrome_options)

    def tearDown(self):
        self.driver.quit()

    def test1(self):
        self.driver.get(url)
        button = self.driver.find_element(By.LINK_TEXT, 'About')
        button.click()
        self.assertEqual(self.driver.current_url, url + "about/")

    def test2(self):
        self.driver.get(url)
        button = self.driver.find_element(By.LINK_TEXT, 'Cities')
        button.click()
        self.assertEqual(self.driver.current_url, url + "cities/")

    def test3(self):
        self.driver.get(url)
        button = self.driver.find_element(By.LINK_TEXT, 'Shelter & Relief')
        button.click()
        self.assertEqual(self.driver.current_url, url + "shelter/")

    def test4(self):
        self.driver.get(url)
        button = self.driver.find_element(By.LINK_TEXT, 'Pharmacies')
        button.click()
        self.assertEqual(self.driver.current_url, url + "pharmacies/")

    def test_city_card(self):
        self.driver.get(url + 'cities')
        self.driver.implicitly_wait(5)
        button = self.driver.find_element(By.XPATH, '/html/body/div/div/div/div[3]/div[1]/div/a/div/div[2]')
        button.click()
        self.assertEqual(self.driver.current_url, url + "cities/1")


    def test_shelter_card(self):
        self.driver.get(url + 'shelter')
        self.driver.implicitly_wait(5)
        button = self.driver.find_element(By.XPATH, '/html/body/div[1]/div/div[3]/div[8]/div/a/div/div[2]')
        button.click()
        self.assertEqual(self.driver.current_url, url + "shelter/8")

    def test_pharmacy_card(self):
        self.driver.get(url + 'pharmacies')
        self.driver.implicitly_wait(5)
        button = self.driver.find_element(By.XPATH, '/html/body/div/div/div/div[3]/div[2]/div[1]/a/div/div[2]')
        button.click()
        self.assertEqual(self.driver.current_url, url + "pharmacies/4")

    def test_next_button_shelter(self):
        self.driver.get(url +'shelter')
        self.driver.implicitly_wait(5)
        button = self.driver.find_element(By.XPATH, '/html/body/div/div/div[4]/div/button[2]')
        button.click()
        self.assertEqual(self.driver.current_url, url + 'shelter/')

    def test_prev_button_city(self):
        self.driver.get(url + 'cities')
        self.driver.implicitly_wait(5)
        button = self.driver.find_element(By.XPATH, '/html/body/div/div/div/div[4]/div/button[1]')
        button.click()
        self.assertEqual(self.driver.current_url, url + 'cities/')

    def test_home_navbar(self):
        self.driver.get(url + 'cities')
        self.driver.implicitly_wait(5)
        button = self.driver.find_element(By.LINK_TEXT, 'StormShelters')
        button.click()
        self.assertEqual(self.driver.current_url, url)

    def test_sitewide_search(self):
        self.driver.get(url)
        self.driver.implicitly_wait(5)
        button = self.driver.find_element(By.XPATH, '/html/body/div/div/div[1]/div/div/div/form/input')
        button.click()
        self.assertEqual(self.driver.current_url, url)

    def test_city_search(self):
        self.driver.get(url + 'cities')
        self.driver.implicitly_wait(5)
        button = self.driver.find_element(By.XPATH, '/html/body/div/div/div/div[2]/form/input')
        button.click()
        self.assertEqual(self.driver.current_url, url + 'cities/')

    def test_sort(self):
        self.driver.get(url + 'cities')
        dropdown = Select(self.driver.find_element(By.XPATH, "/html/body/div/div/div/div[1]/div/div[1]/div/select"))
        dropdown.select_by_index(1)
        self.driver.implicitly_wait(5)
        first_card_title = self.driver.find_element(
            By.XPATH, "/html/body/div/div/div/div[3]/div[1]/div/a/div/div[2]/h2"
        ).text
        self.assertEqual(first_card_title, "Baytown")




if __name__ == "__main__":
    unittest.main()
