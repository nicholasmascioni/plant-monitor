/*
Test the soil at different moisture levels to determine the threshold value to use
Start with dry soil and gradually add water, taking note of the values on the serial monitor
Use the results to determine a threshold the soil must meet to be considered moist enough
*/

#define SENSOR 36

void setup()
{
  pinMode(SENSOR, INPUT);
  
  Serial.begin(9600);
}

void loop()
{
  int moisture = analogRead(SENSOR);
  
  Serial.println(moisture);
  delay(500);
}