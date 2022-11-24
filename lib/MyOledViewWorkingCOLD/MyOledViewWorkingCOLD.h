/**
    Class MyOledViewWorkingCOLD : Gestion d'une VUE pour la température
    @file MyOledViewWorkingCOLD.h 
    @author GUILLOU Quentin
    @version 1.1 11/11/22
**/


#ifndef MYOLEDVIEWWORKINGCOLD_H
#define MYOLEDVIEWWORKINGCOLD_H

#include <Adafruit_SSD1306.h>
#include <string>

#include "MyOledViewWorking.h"

class MyOledViewWorkingCOLD : public MyOledViewWorking
{
    private :
        void display( Adafruit_SSD1306 *adafruit);
        void update(Adafruit_SSD1306 *adafruit);
};
#endif