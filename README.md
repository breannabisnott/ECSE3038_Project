# ECSE3038 Final Project - Breanna Bisnott

## api

@app.put("/settings", status_code=200): put request to post/update the user-specified settings from the website

@app.get("/graph", status_code=200): get request to take sensor data from database to use to plot graph on website

@app.post("/sensorData", status_code=201): post request from ESP to post sensor data database

@app.get("/fan", status_code=200): get request to compare user-specified settings and sensor data to determine if fan should turn on or off

@app.get("/light", status_code=200): get request to compare user-specified settings and sensor data to determine if light should turn on or off

## embeded

void post_sensor_data(float temp, bool presence): this function posts the sensor data gathered from the ESP to the database through the API

void get_fan_data(): function changes the state of fan (using the ESP) dependent on the logic calculated by the api get request

void get_light_data(): function changes the state of light (using the ESP) dependent on the logic calculated by the api get request

void setup(): connect to WIFI and initialize output/input pins

void loop(): prompts ESP to get temp and motion sensor data & prompts functions to change state of fan and light.
