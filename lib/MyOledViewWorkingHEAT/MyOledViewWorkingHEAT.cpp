/**
    Gestion d'une VUE pour le OLed  
    @file MyOledViewWorkingHEAT.cpp
    @author GUILLOU Quentin
    @version 1.1 11/11/22
*/
#include <Arduino.h>
#include "MyOledViewWorkingHEAT.h"

using namespace std;

void MyOledViewWorkingHEAT::display(Adafruit_SSD1306 *adafruit)
{
    adafruit->clearDisplay();

    adafruit->setTextSize(2);
    adafruit->setCursor(0, 0);
    adafruit->print(getTag("nomDuSysteme").c_str());

    adafruit->setTextSize(1);
    adafruit->setCursor(0, 20);
    adafruit->print(getTag("idDuSysteme").c_str());
    
    adafruit->setCursor(40, 50);
    adafruit->print(getTag("ipDuSysteme").c_str());

    adafruit->setCursor(80, 20);
    adafruit->print("Heating");
    adafruit->setTextSize(2);
    adafruit->setCursor(10, 30);
    adafruit->print(getTag("temperatureDuSysteme").c_str());

    adafruit->display();;

}
void MyOledViewWorkingHEAT::update(Adafruit_SSD1306 *adafruit)
{
    Serial.println("Update my view MyOledViewWorkingHEAT");
    display(adafruit);
}