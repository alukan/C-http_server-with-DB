# Install dependencies

### Linux

`sudo apt install libmongoc-dev libbson-dev`

### Mac OS

`brew install mongo-c-driver`

# build

`make`

# run
`./build/bin/ServerApp`

# what it can do

## routes:

`GET /` - welcome page

`GET /about` - to show that server can distinguish between / and /about (not really needed after all changes)

`GET /count/{num}/addition` - performs "num" amount of addition operations and sends how long it took. "num" is query parameter.

`GET /count/{num}/multiplication` - performs "num" amount of multiplication operations and sends how long it took. "num" is query parameter.

`POST /` - takes body and sends it back.

`GET /file/{name}` - get file with name "name" from mongodb.

`POST /file` - save file to mongodb. Body is JSON that should have "name" and "content" fields.

`PUT /file` - update file in mongodb. Body is JSON that should have "name" and "content" fields.

`DELETE /file/{name}` - delete file with name "name" from mongodb.

### Notes:

http server based on TCP. Works with http 1.1

Can handle:

- GET, POST, PUT, DELETE
- JSON body
- Text body (text/plain)
- query parameters
- 404 routes
- 500 errors
- .env
- Connection to MongoDB. With insert, update, delete

To make /file routes work you need to add mongo_uri and database in .env.