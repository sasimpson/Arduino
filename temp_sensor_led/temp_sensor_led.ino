#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 10

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

struct RGB {
  byte red;
  byte green;
  byte blue;
};

struct temps {
  float min;
  float max;
  RGB color;
};

int RED   = 6;
int GREEN = 3;
int BLUE  = 5;

int temps_table_length = 11;
temps temps_table[] = {
  {68,70,{0,255,192}},
  {70,72,{0,255,128}},
  {72,74,{0,255,64}},
  {74,76,{64,255,0}},
  {76,78,{128,255,0}},
  {78,80,{192,255,0}},
  {80,82,{255,255,0}},
  {82,84,{255,192,0}},
  {84,86,{255,128,0}},
  {86,88,{255,64,0}},
  {88,90,{255,0,0}}
  // {,,{}},
};

//ghetto way to get around not being able to pass structs into functions
void color(void *ptr) {
  RGB *c;
  c = (RGB *)ptr;
  analogWrite(GREEN, 255-c->green);
  analogWrite(BLUE, 255-c->blue);
  analogWrite(RED, 255-c->red);
}

void color_for_temps(float t) {
  for (int i = 0; i < temps_table_length; i++) {
    temps ts = temps_table[i];
    if (t > ts.min & t <= ts.max) {
      color(&ts.color);
      return;
    }
  }
}

void setup(void)
{
  Serial.begin(9600);
  sensors.begin();
}

void loop(void)
{ 
  digitalWrite(12, HIGH);
  sensors.requestTemperatures(); // Send the command to get temperatures
  float temp = sensors.getTempFByIndex(0);
  color_for_temps(temp);
  Serial.print(temp); 
  Serial.print("\r\n");
  delay(10);
}
