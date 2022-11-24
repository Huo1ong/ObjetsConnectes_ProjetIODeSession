/**
    Class MyOledViewInitialisation : Gestion d'une VUE pour le OLed lors de l'initialisation
    @file MyOledViewInitialisation.h 
    @author GUILLOU Quentin
    @version 1.1 11/11/22 
**/


#ifndef MYOLEDVIEWINITIALISATION_H
#define MYOLEDVIEWINITIALISATION_H

#include "MyOledView.h"

#include <Adafruit_SSD1306.h>
#include <string>

class MyOledViewInitialisation : public MyOledView
{
    public :
        void setNomDuSysteme(std::string val);
        void setIdDuSysteme(std::string val);
        void setSensibiliteBoutonAction(std::string val);
        void setSensibiliteBoutonReset(std::string val);

    private :
        void display(Adafruit_SSD1306 *adafruit);
        void update(Adafruit_SSD1306 *adafruit);

        std::string nomdusysteme;
        std::string iddusysteme;
        std::string sensibiliteboutonaction;
        std::string sensibiliteboutonreset;
};
#endif