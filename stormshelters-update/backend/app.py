from flask import jsonify, request
from models import db, app, Pharmacy, Shelter, City
from schema import city_schema, pharmacy_schema, shelter_schema
from sqlalchemy import or_
from sqlalchemy import and_


@app.route('/')
def home():
    return "API Home"

# get model page of cities
@app.route('/cities',  methods=['GET'])
def get_cities():

    query = db.session.query(City)
    page = request.args.get('page', type=int, default=1)
    per_page = request.args.get('per_page', type=int, default=9)

    # city, pop, temp
    sort = request.args.get('sort')
    # asc, dec
    order = request.args.get('order')

    # conditions, precipitation
    condition = request.args.get('condition', default=None)
    precipitation = request.args.get('precipitation', default=None)

    if condition is not None:
        query = query.filter(City.cond.like("%" + condition + "%"))
    if precipitation is not None:
        if precipitation == "Light":
            query = query.filter((City.precip_in < 2))
        elif precipitation == "Medium":
            query = query.filter(City.precip_in < 5)
            query = query.filter(City.precip_in >= 2)
        else: #precipitation == "Heavy"
            query = query.filter(City.precip_in >= 5)


    # sort every entry by city name in asc
    if sort == 'city' and order == 'asc':
        query = query.order_by(City.name)
    elif sort == 'city' and order == 'desc':
        query = query.order_by(City.name.desc())
    elif sort == 'pop' and order == 'asc':
        query = query.order_by(City.pop)
    elif sort =='pop' and order =='desc':
        query = query.order_by(City.pop.desc())
    elif sort == 'temp' and order == 'asc':
        query = query.order_by(City.temp_in_f)
    elif sort == 'temp' and order == 'desc':
        query = query.order_by(City.temp_in_f.desc())
    elif sort == 'wind' and order =='asc':
        query = query.order_by(City.wind_mph)
    elif sort == 'wind' and order == 'desc':
        query = query.order_by(City.wind_mph.desc())

    result = query.paginate(page=page, per_page=per_page, error_out=False) 
    schema = city_schema.dump(result, many=True)
 
    total = query.count()     
    return jsonify({"cities" : schema,
                    "meta": {
                        "count": total
                    }})

# get model page of pharmacies
@app.route('/pharmacies',  methods=['GET'])
def get_pharmacy():
    query = db.session.query(Pharmacy)
    page = request.args.get('page', 1, int)
    per_page = request.args.get('per_page', 9, int)
    
    #name, city, dist
    sort = request.args.get('sort')
    order = request.args.get('order')

    #category and city
    name = request.args.get('name')
    city = request.args.get('city')

    #filtering
    if name is not None:
        query = query.filter(Pharmacy.name.like("%" + name + "%"))
    if city is not None:
        query = query.filter(Pharmacy.city.like("%" + city + "%"))
    
    #sorting
    if sort == 'name' and order == 'asc':
        query = query.order_by(Pharmacy.name)
    elif sort == 'name' and order == 'desc':
        query = query.order_by(Pharmacy.name.desc())
    elif sort == 'city' and order == 'asc':
        query = query .order_by(Pharmacy.city)
    elif sort == 'city' and order == 'desc':
        query = query.order_by(Pharmacy.city.desc())
    elif sort == 'dist' and order == 'asc':
        query = query.order_by(Pharmacy.distance_m)
    elif sort == 'dist' and order == 'desc':
        query = query.order_by(Pharmacy.distance_m.desc())

    result = query.paginate(page=page, per_page=per_page, error_out=False) 
    schema_dump = pharmacy_schema.dump(result, many=True)
    total = query.count()     
    return jsonify({"pharmacies" : schema_dump,
                    "meta": {
                        "count": total
                    }})

# get model page of shelters
@app.route('/shelters',  methods=['GET'])
def get_shelters():
    page = request.args.get('page', 1, int)
    per_page = request.args.get('per_page', 9, int)
    query = db.session.query(Shelter)
    sort = request.args.get('sort')
    order = request.args.get('order')

    closed = request.args.get('closed')
    rating = request.args.get('rating')

    #filtering
    if closed is not None:
        if closed == 'Yes':
            query = query.filter(Shelter.is_closed == True)
        if closed == 'No':
            query = query.filter(Shelter.is_closed != True)
    if rating is not None:
        if rating == '0-1':
            query = query.filter(Shelter.rating <= 1)
        elif rating == '1-2':
            query = query.filter(Shelter.rating <= 2)
            query = query.filter(Shelter.rating > 1)
        elif rating == '2-3':
            query = query.filter(Shelter.rating <= 3)
            query = query.filter(Shelter.rating > 2)
        elif rating == '3-4':
            query = query.filter(Shelter.rating <= 4)
            query = query.filter(Shelter.rating > 3)
        elif rating == '4-5':
            query = query.filter(Shelter.rating > 4)
        


    if sort == 'name' and order == 'asc':
        query = query.order_by(Shelter.name)
    elif sort == 'name' and order == 'desc':
        query = query.order_by(Shelter.name.desc())
    elif sort == 'rating' and order == 'asc':
        query = query.order_by(Shelter.rating)
    elif sort == 'rating' and order == 'desc':
        query = query.order_by(Shelter.rating.desc())
    elif sort == 'city' and order == 'asc':
        query = query.order_by(Shelter.city)
    elif sort == 'city' and order == 'desc':
        query = query.order_by(Shelter.city.desc())


    result = query.paginate(page=page, per_page=per_page, error_out=False) 
    schema_dump = shelter_schema.dump(result, many=True)

    total = query.count()

    return jsonify({"shelters" : schema_dump,
                    "meta": {
                        "count": total
                    }})

# get instance of city
@app.route('/cities/<int:city_id>', methods=['GET'])
def get_single_city(city_id):
    try:
        city = db.session.query(City).filter_by(id=city_id).one()
        city_data = city_schema.dump(city)

        return jsonify({"city": city_data})
    except IndexError:
        return jsonify({"error": "City not found"}, 404)

# get instance of shelter
@app.route('/shelters/<int:shelter_id>', methods=['GET'])
def get_single_shelter(shelter_id):
    try:
        shelter = db.session.query(Shelter).filter_by(id=shelter_id).one()
        shelter_data = shelter_schema.dump(shelter)

        return jsonify({"shelter": shelter_data})
    except IndexError:
        return jsonify({"error": "Shelter/Food not found"}, 404)

# get instance of pahrmacy
@app.route('/pharmacies/<int:pharm_id>', methods=['GET'])
def get_single_pharmacy(pharm_id):
    try:
        pharmacy = db.session.query(Pharmacy).filter_by(id=pharm_id).one()
        pharm_data = pharmacy_schema.dump(pharmacy)

        return jsonify({"pharmacy": pharm_data})
    except IndexError:
        return jsonify({"error": "Pharmacy not found"}, 404) 
    
# site wide searching - different search bar
# and model specific searching 
@app.route("/search/<string:model>/<string:query>", methods=['GET'])
def search_models(model, query):
    terms = query.split()
    model = model.strip().lower()
    if model == "city":
        freq = search_cities(terms)
        cities = sorted(freq.keys(), key = lambda x: freq[x], reverse = True)
        result = city_schema.dump(cities, many=True)
    elif model == 'shelter':
        freq = search_shelters(terms)
        shelters = sorted(freq.keys(), key = lambda x: freq[x], reverse = True)
        result = shelter_schema.dump(shelters, many=True)
    elif model == "pharmacies":
        freq = search_pharmacies(terms)
        pharmacies = sorted(freq.keys(), key = lambda x: freq[x], reverse = True)
        result = pharmacy_schema.dump(pharmacies, many=True)
    elif model == 'all':
        freq = {
        **search_cities(terms),
        **search_shelters(terms),
        **search_pharmacies(terms)
        }
        order = sorted(freq.keys(), key = lambda x: freq[x], reverse = True)
        cities = [city for city in order if type(city) == City]
        shelters = [shelter for shelter in order if type(shelter) == Shelter]
        pharmacies = [pharmacy for pharmacy in order if type(pharmacy) == Pharmacy]

        city_results = city_schema.dump(cities, many = True)
        shelter_results = shelter_schema.dump(shelters, many = True)
        pharmacy_results = pharmacy_schema.dump(pharmacies, many=True)

        return jsonify({
            "cities": city_results,
            "shelters":shelter_results,
            "pharmacies":pharmacy_results
        })
    else: 
        jsonify({"Invalid model": 400})
    return jsonify({"data": result})

        

# returns cities consiting of search terms
def search_cities(terms):
    freq = {}
    for term in terms:
        queries = []
        queries.append(City.name.contains(term))
        queries.append(City.temp_in_f.contains(term))
        queries.append(City.pop.contains(term))
        queries.append(City.cond.contains(term))
        queries.append(City.wind_mph.contains(term))
        cities = City.query.filter(or_(*queries))
        for city in cities:
            if city not in freq:
                freq[city] = 1
            else:
                freq[city] += 1
    return freq

# returns shelters consisting of search terms
def search_shelters(terms):
    freq = {}
    for term in terms:
        queries = []
        queries.append(Shelter.city.contains(term))
        queries.append(Shelter.display_address.contains(term))
        queries.append(Shelter.display_phone.contains(term))
        queries.append(Shelter.name.contains(term))
        queries.append(Shelter.image_url.contains(term))
        queries.append(Shelter.latitude.contains(term))
        queries.append(Shelter.longitude.contains(term))
        queries.append(Shelter.url.contains(term))
        shelters = Shelter.query.filter(or_(*queries))
        for sh in shelters:
            if sh not in freq:
                freq[sh] = 1
            else:
                freq[sh] += 1
    return freq

# returns pharmacies consisting of search terms
def search_pharmacies(terms):
    freq = {}
    for term in terms:
        queries = []
        queries.append(Pharmacy.city.contains(term))
        queries.append(Pharmacy.address.contains(term))
        queries.append(Pharmacy.name.contains(term))
        queries.append(Pharmacy.categories.contains(term))
        queries.append(Pharmacy.distance_m.contains(term))
        pharmacies = Pharmacy.query.filter(or_(*queries))
        for ph in pharmacies:
            if ph not in freq:
                freq[ph] = 1
            else:
                freq[ph] += 1
    return freq


    




def split_strings_city(city):
    city['shelters'] = list(filter(None,city['shelters'].split(';')))
    city['pharmacies'] = list(filter(None,city['pharmacies'].split(';')))

def split_strings_pharmacy(pharmacy):
    pharmacy['shelters'] = list(filter(None,pharmacy['shelters'].split(';')))
    pharmacy['cities'] = list(filter(None,pharmacy['cities'].split(';')))

def split_strings_shelter(shelter):
    shelter['pharmacies'] = list(filter(None,shelter['pharmacies'].split(';')))
    shelter['cities'] = list(filter(None,shelter['cities'].split(';')))




if __name__ == '__main__':
    app.run(port=5000, debug=True)
