#schema for json that is returned from RESTful API
from pprint import pprint
from flask_marshmallow import Marshmallow

from models import City, Pharmacy, Shelter
from marshmallow_sqlalchemy import SQLAlchemyAutoSchema, fields

ma = Marshmallow()

class CitySchema(SQLAlchemyAutoSchema):
    class Meta:
        model = City

class ShelterSchema(SQLAlchemyAutoSchema):
    class Meta:
        model = Shelter
        
class PharmacySchema(SQLAlchemyAutoSchema):
    class Meta:
        model = Pharmacy

city_schema = CitySchema()
shelter_schema = ShelterSchema()
pharmacy_schema = PharmacySchema()