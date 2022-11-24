/**
    Class MyOledViewWifiAp : Gestion d'une VUE pour le OLed
    @file MyOledViewWifiAp.h 
    @author GUILLOU Quentin
    @version 1.1 11/11/22
**/


#ifndef MYOLEDVIEWWIFIAP_H
#define MYOLEDVIEWWIFIAP_H

#include <Adafruit_SSD1306.h>
#include <string>

#include "MyOledView.h"

class MyOledViewWifiAp : public MyOledView
{
    public :
        void setNomDuSysteme(std::string val);
        void setSsIdDuSysteme(std::string val);
        void setPassDuSysteme(std::string val);

    private :
        void display( Adafruit_SSD1306 *adafruit);
        void update(Adafruit_SSD1306 *adafruit);
        std::string nomdusysteme;
        std::string ssiddusysteme;
        std::string passdusysteme;
};
#endif