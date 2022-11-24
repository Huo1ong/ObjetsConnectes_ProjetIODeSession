/**
    Gestion d'une VUE pour le OLed  
    @file MyOledViewWorkingOFF.cpp
    @author GUILLOU Quentin - LECOMTE Guillaume
    @version 1.1 21/09/20 
*/
#include <Arduino.h>
#include "MyOledViewWorkingOFF.h"

using namespace std;

  void MyOledViewWorkingOFF::display(Adafruit_SSD1306 *adafruit)
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
    adafruit->print("Ready");
    adafruit->setCursor(80, 30);
    adafruit->print(getTag("temperatureDuSysteme").c_str());

    adafruit->display();
}