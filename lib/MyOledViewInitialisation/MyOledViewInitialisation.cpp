/**
    Gestion d'une VUE pour le OLed  
    @file MyOledViewInitialisation.cpp
    @author GUILLOU Quentin
    @version 1.1 11/11/22 
*/
#include <Arduino.h>
#include "MyOledViewInitialisation.h"
using namespace std;

void MyOledViewInitialisation::setNomDuSysteme(std::string val){
    nomdusysteme = val;
}
void MyOledViewInitialisation::setIdDuSysteme(std::string val){
    iddusysteme = val; 
}

void MyOledViewInitialisation::setSensibiliteBoutonAction(std::string val){
    sensibiliteboutonaction = val;
}

void MyOledViewInitialisation::setSensibiliteBoutonReset(std::string val){
    sensibiliteboutonreset = val;
}


void MyOledViewInitialisation::display(Adafruit_SSD1306 *adafruit)
{

    char strAction[120];
    char strReset[120];
    sprintf(strAction, "Bouton ACTION : %s", sensibiliteboutonaction.c_str());
    sprintf(strReset, "Bouton RESET : %s", sensibiliteboutonreset.c_str());

    adafruit->clearDisplay();

    adafruit->setCursor(0, 0);
    adafruit->setTextSize(2);
    adafruit->println("SAC system");
    adafruit->setTextSize(1);
    adafruit->println("Id : ID1303");
    adafruit->println("INITIALISATION");
    adafruit->println(strAction);
    adafruit->println(strReset);

    adafruit->display();
}

void MyOledViewInitialisation::update(Adafruit_SSD1306 *adafruit)
{
    display(adafruit);
}