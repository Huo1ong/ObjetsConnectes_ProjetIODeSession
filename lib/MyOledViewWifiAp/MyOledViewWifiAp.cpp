/**
    Gestion d'une VUE pour le OLed lié au wifi 
    @file MyOledViewWifiAp.cpp
    @author GUILLOU Quentin
    @version 1.1 11/11/22 
*/

#include <Arduino.h>
#include "MyOledViewWifiAp.h"

using namespace std;

void MyOledViewWifiAp::setNomDuSysteme(std::string val)
{
    nomdusysteme = val;
}

void MyOledViewWifiAp::setSsIdDuSysteme(std::string val)
{
     ssiddusysteme = val;
}

void MyOledViewWifiAp::setPassDuSysteme(std::string val)
{
     passdusysteme = val; 
}

void MyOledViewWifiAp::display(Adafruit_SSD1306 *adafruit)
{

    char strSsid[120];
    char strPass[120];
    sprintf(strSsid, "SSID : %s", ssiddusysteme.c_str());
    sprintf(strPass, "PASS  : %s", passdusysteme.c_str());

    adafruit->clearDisplay();

    adafruit->setCursor(0, 0);
    adafruit->setTextSize(2);
    adafruit->println("SAC system");
    adafruit->setTextSize(1);
    adafruit->println("Id : Sac System");
    adafruit->println("Ap Configuration");
    adafruit->println(strSsid);
    adafruit->println(strPass);

    adafruit->display();
}

void MyOledViewWifiAp::update(Adafruit_SSD1306 *adafruit)
{
    display(adafruit);
}