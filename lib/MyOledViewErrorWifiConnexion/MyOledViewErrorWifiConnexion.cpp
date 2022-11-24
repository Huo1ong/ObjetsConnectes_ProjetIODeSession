/**
    Gestion d'une VUE pour le OLed  
    @file MyOledViewErrorWifiConnexion.cpp
    @author GUILLOU Quentin
    @version 1.1 11/11/22 
*/
#include <Arduino.h>
#include "MyOledViewErrorWifiConnexion.h"

using namespace std;


void MyOledViewErrorWifiConnexion::setNomDuSysteme(string val)
{
    nomdusysteme = val;
}