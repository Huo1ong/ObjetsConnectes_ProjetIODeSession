/**
    Class MyOledViewWorkingHEAT : Gestion d'une VUE pour la température
    @file MyOledViewWorkingHEAT.h 
    @author GUILLOU Quentin
    @version 1.1 11/11/22
**/


#ifndef MYOLEDVIEWWORKINGHEAT_H
#define MYOLEDVIEWWORKINGHEAT_H

#include <Adafruit_SSD1306.h>
#include <string>

#include "MyOledViewWorking.h"

class MyOledViewWorkingHEAT : public MyOledViewWorking
{
    private :
        void display( Adafruit_SSD1306 *adafruit);
        void update(Adafruit_SSD1306 *adafruit);
};
#endif