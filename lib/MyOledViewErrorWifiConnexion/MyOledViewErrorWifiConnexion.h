/**
    Class MyOledViewWifiErrorConnexion : Gestion d'une VUE pour le OLed
    @file MyOledViewWifiErrorConnexion.h 
    @author GUILLOU Quentin
    @version 1.1 11/11/22 
    
    Historique des versions   
                            Versions  Date      Auteur      Description
                            1.1      21/09/20   Ald         Première version de la classe
                            
                            
    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino
    lib_deps = 
        Aucune
    Autres librairies (à copier dans le répertoire lib)
        Aucune
    
    Exemple d'utilisation 
        //Définition 
            #include "MyOledView.h"
            class MyOledViewInitialisation: public MyOledView {
                public:
                
                private:
                    //Mettre la vue dans cette méthode
                    void display( Adafruit_SSD1306 *adafruit);
        ...
        //Utilisation
        myOledViewInitialisation = new MyOledViewInitialisation();
        myOled->displayView(myOledViewInitialisation);
**/


#ifndef MYOLEDVIEWERRORWIFICONNEXION_H
#define MYOLEDVIEWERRORWIFICONNEXION_H

#include "MyOledView.h"

#include <Adafruit_SSD1306.h>
#include <string>


class MyOledViewErrorWifiConnexion : public MyOledView
{
    public :
        void setNomDuSysteme(std::string val);

    private :
        int findVal(std::string tag);

        virtual void display( Adafruit_SSD1306 *adafruit) = 0;
        virtual void update(Adafruit_SSD1306 *adafruit) = 0;
        std::string nomdusysteme;

};
#endif