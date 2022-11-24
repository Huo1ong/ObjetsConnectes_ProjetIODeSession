/**
    Class MyOledViewWorkingOFF
    @file MyOledViewWorkingOFF.h 
    @author GUILLOU Quentin
    @version 1.1 11/11/22
**/


#ifndef MYOLEDVIEWWORKINGOFF_H
#define MYOLEDVIEWWORKINGOFF_H

#include <Adafruit_SSD1306.h>
#include <string>

#include "MyOledViewWorking.h"

class MyOledViewWorkingOFF : public MyOledViewWorking
{
    private :
        void display( Adafruit_SSD1306 *adafruit);
};
#endif