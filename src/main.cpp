/* Copyright (C) 2022
 * All rights reserved.
 *
 * Projet Sac
 * Ecole du Web
 * Cours Objets connectés (c)2022
 *  
    @file     main.cpp
    @author   Alain Dubé
    @version  1.2 03/11/22 

    Historique des versions
           Version    Date       Auteur       Description
           1.1        21/08/15  Alain       Première version du logiciel
           1.2        03/11/22  Quentin     Seconde version du logiciel

    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino
    lib_deps =  
            ESPAsyncWebServer-esphome                   (Pour accéder au Wifi)
            AsyncTCP-esphome                            (Pour utiliser les focntionnalités TCP)
            bblanchon/ArduinoJson@^6.17.2               (Pour accéder au fonctionnalités Json)

    Autres librairies (à copier dans le répertoire lib)
         WifiManagerDevelopment
            //Remarques
            //Pour trouver le WifiManager (dans la branche development)
            //   https://github.com/tzapu/WiFiManager/tree/development
            //   Ne pas oublier d'appuyez sur l'ampoule et choisir : ajouter Lib
    
    Fonctions utiles (utilitaires)
        /lib/MYLIB/myFunctions.cpp
            //Pour vérifier plus simplement que deux chaines sont identiques
            bool isEqualString(std::string line1, std::string line2)
            //Pour extraire une partie d'une chaine de caractères avec l'aide d'un index
            std::string getValue(std::string data, char separator, int index)
            //Pour remplacer plus facilement une sous chaine
            bool replaceAll(std::string& source, const std::string& from, const std::string& to)
            //Pour obtenir un chaine aléatoire d'une certaine longeur
            std::string get_random_string(unsigned int len)

    Classes du système
         
        MyServer                        V1.0    Pour la gestion des routes pour le site WEB
            /data                               Répertoire qui contient les fichiers du site WEB 
                index.html              V1.0    Page index du site WEB
                index.css               V1.0    CSS
                script.js               V1.0    JS (fonctions JavaScript)
              
 * */
using namespace std;

#include <iostream>
#include <string>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <HTTPClient.h>
HTTPClient http;

#include <WiFiManager.h>
WiFiManager wm;
#define WEBSERVER_H

#include <WifiClient.h>

  // My code moved from loop() ...
  WiFiClient client;

//Pour avoir les données du senseur de température
#include "TemperatureStub.h"
#define DHTPIN  15                 // Pin utilisée par le senseur / DHT22
#define DHTTYPE DHT22              //Le type de senseur utilisé (mais ce serait mieux d'avoir des DHT22 pour plus de précision)
TemperatureStub *temperatureStub = NULL;

//Pour la gestion du serveur ESP32
#include "MyServer.h"
MyServer *myServer = NULL;

//Variable pour la connection Wifi
const char *SSID = "SAC_";
const char *PASSWORD = "sac_";
String ssIDRandom;

//Écran OLED
#define SCREEN_WIDTH 128        // OLED display width, in pixels
#define SCREEN_HEIGHT 64        // OLED display height, in pixels
#define OLED_RESET 4            // Reset pin # (or -1 if sharing Arduino reset pin)
#define OLED_I2C_ADDRESS 0x3C   // Adresse I2C de l'écran Oled

//LEDS 
//Définition des trois leds de statut
#define GPIO_PIN_LED_OK_VERTE 14           //GPIO14 - VERTE
#define GPIO_PIN_LED_HEAT_JAUNE 27         //GPIO27 - JAUNE
#define GPIO_PIN_LED_LOCK_ROUGE 12         //GPIO12 - ROUGE

//Fonctions utilitaires
#include "myFunctions.cpp" 

//Classes
#include "MyOled.h"
    MyOled *myoled = new MyOled(&Wire,OLED_RESET, SCREEN_HEIGHT, SCREEN_WIDTH);
#include "MyOledView.h"
    MyOledView *myoledview = NULL;
#include "MyOledViewInitialisation.h"
    MyOledViewInitialisation *myoledviewinitialisation = NULL;
#include "MyOledViewWorking.h"
    MyOledViewWorking *myoledviewworking = NULL;
#include "MyOledViewWifiAp.h"
    MyOledViewWifiAp *myoledviewwifiap = NULL;
#include "MyOledViewWorkingOFF.h"
    MyOledViewWorkingOFF *myoledviewworkingoff = NULL;
#include "MyOledViewWorkingCOLD.h"
    MyOledViewWorkingCOLD *myoledviewworkingcold = NULL;
#include "MyOledViewWorkingHEAT.h"
    MyOledViewWorkingHEAT *myoledviewworkingheat = NULL;
#include "MyOledViewErrorWifiConnexion.h"
    MyOledViewErrorWifiConnexion *myoledviewerrorwificonnexion = NULL;

//Boutons Action et Reset
#include "MyButton.h"
MyButton *myButtonAction = NULL;
MyButton *myButtonReset = NULL;

//Variables utilisées
String JsonListeBois;
String JsonLeBois;
std::string tempDuFour = "22";              //Lire le senseur de température
boolean btnFour = false;                    //Bouton démarrage du four
int delayTimer = 0;                         //Pour compter le nombre de secondes depuis le début
int etatDuFour = 0;                         //État du four à '0' pour commencer (3 états prédéfinis : 0 = OFF, 1 = COLD et 2 = HEAT)
string nomDuSysteme = "SAC system";         //Nom du système prédéfini
string idDuSysteme = "Id: 1303";            //Id du système prédéfini
char buffer[100];
float temperature = 0;
float temperatureDemandee = 28;
int nbrDeSecondes = 20;
int tempsDeSechage = 20;


//fonction statique qui permet aux objets d'envoyer des messages (callBack) 
//  arg0 : Action 
// arg1 ... : Parametres
std::string CallBackMessageListener(string message) {
    while(replaceAll(message, std::string("  "), std::string(" ")));
    //Décortiquer le message
    string actionToDo = getValue(message, ' ', 0);
    string arg1 = getValue(message, ' ', 1);
    string arg2 = getValue(message, ' ', 2);
    string arg3 = getValue(message, ' ', 3);
    string arg4 = getValue(message, ' ', 4);
    string arg5 = getValue(message, ' ', 5);
    string arg6 = getValue(message, ' ', 6);
    string arg7 = getValue(message, ' ', 7);
    string arg8 = getValue(message, ' ', 8);
    string arg9 = getValue(message, ' ', 9);
    string arg10 = getValue(message, ' ', 10);    

    //Obtenir température ambiante
    if (string(actionToDo.c_str()).compare(string("askTempFour")) == 0)
    {
        temperature = temperatureStub->getTemperature();
        //sprintf(buffer, "%4.1f", temperature);
        sprintf(buffer, "%4.1f °C;%is", temperature, nbrDeSecondes);
        return(buffer);
    }

    //Bouton "Démarrage du four"
    if (string(actionToDo.c_str()).compare(string("btnFour")) == 0)
    {
       Serial.println("Démarrage du four.");
       btnFour = true;
       return "ok";
    }

    if (string(actionToDo.c_str()).compare(string("askListeWood")) == 0) {
        http.begin(client, "http://149.56.141.62:3000/api/woods/getAllWoods");
        http.GET();
        JsonListeBois = http.getString();
        Serial.println(JsonListeBois);
        http.end();
        return(JsonListeBois.c_str()); }

    if (string(actionToDo.c_str()).compare(string("afficherBois")) == 0) {
        char buffer[100];
        sprintf(buffer, "http://149.56.141.62:3000/api/woods/getWood/%S", arg1.c_str());
        http.begin(client, buffer);
        http.addHeader("Authorization","eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE2Njg3ODUyMjF9.jhT6LpcaUyx5w0gXGldjC9TZxymvArrzvPt6GG2WukM");
        http.GET();
        JsonLeBois = http.getString();
        Serial.println(JsonLeBois);
        http.end();
        DynamicJsonDocument doc(2048);
        deserializeJson(doc,JsonLeBois);
        for(JsonObject elem : doc.as<JsonArray>()){
            temperatureDemandee = elem["temperature"];
            tempsDeSechage = elem["dryingTime"];
            nbrDeSecondes = tempsDeSechage;
        }
        return(JsonLeBois.c_str()); }


    std::string result = "";
    return result;
    }

void NouveauParametre()
{
    switch(etatDuFour)
    {
        case 0:   //État OFF
            myoledviewworkingoff = new MyOledViewWorkingOFF();
            myoledviewworkingoff->setParams("nomDuSysteme", nomDuSysteme.c_str());
            myoledviewworkingoff->setParams("idDuSysteme", idDuSysteme.c_str());
            myoledviewworkingoff->setParams("ipDuSysteme", WiFi.localIP().toString().c_str());
            myoledviewworkingoff->setParams("temperatureDuSysteme",buffer);
            myoled->displayView(myoledviewworkingoff);
            break;

        case 1:   //État COLD
            myoledviewworkingcold = new MyOledViewWorkingCOLD();
            myoledviewworkingcold->setParams("nomDuSysteme", nomDuSysteme.c_str());
            myoledviewworkingcold->setParams("idDuSysteme", idDuSysteme.c_str());
            myoledviewworkingcold->setParams("ipDuSysteme", WiFi.localIP().toString().c_str());
            myoledviewworkingcold->setParams("temperatureDuSysteme",buffer);
            myoled->displayView(myoledviewworkingcold);
            break;
        
        case 2:   //État HEAT
            myoledviewworkingheat = new MyOledViewWorkingHEAT();
            myoledviewworkingheat->setParams("nomDuSysteme", nomDuSysteme.c_str());
            myoledviewworkingheat->setParams("idDuSysteme", idDuSysteme.c_str());
            myoledviewworkingheat->setParams("ipDuSysteme", WiFi.localIP().toString().c_str());
            myoledviewworkingheat->setParams("temperatureDuSysteme",buffer);
            myoled->displayView(myoledviewworkingheat);
            break;
    }
} 

void setup() 
{ 
    Serial.begin(9600);
    delay(100);

    //LEDS
    //Initialisation des LED statuts
    pinMode(GPIO_PIN_LED_OK_VERTE, OUTPUT);
    pinMode(GPIO_PIN_LED_HEAT_JAUNE, OUTPUT);
    pinMode(GPIO_PIN_LED_LOCK_ROUGE, OUTPUT);

    //INITIALISATION ÉCRAN INIT
    myoled->init(OLED_I2C_ADDRESS, true);

    myoledviewinitialisation = new MyOledViewInitialisation();
    myoledviewinitialisation->setNomDuSysteme("SAC system");
    myoledviewinitialisation->setIdDuSysteme("ID : ID1303");
    myoledviewinitialisation->setSensibiliteBoutonAction("????");
    myoledviewinitialisation->setSensibiliteBoutonReset("????");
    myoled->displayView(myoledviewinitialisation);

    //Gestion des boutons
    myButtonAction = new MyButton();        //Pour lire le bouton actions
    myButtonAction->init(T8);
    int sensibilisationButtonAction = myButtonAction->autoSensibilisation();

    myButtonReset = new MyButton();         //Pour lire le bouton hard reset
    myButtonReset->init(T9);
    int sensibilisationButtonReset = myButtonReset->autoSensibilisation();

    myoledviewinitialisation->setSensibiliteBoutonAction(String(sensibilisationButtonAction).c_str());
    myoledviewinitialisation->setSensibiliteBoutonReset(String(sensibilisationButtonReset).c_str());
    myoled->displayView(myoledviewinitialisation);


    Serial.print("sensibilisationButtonAction : "); Serial.println(sensibilisationButtonAction);
    Serial.print("sensibilisationButtonReset : "); Serial.println(sensibilisationButtonReset);

    //Connection au WifiManager
    String ssIDRandom, PASSRandom;
    String stringRandom;
    stringRandom = get_random_string(4).c_str();
    ssIDRandom = SSID;
    ssIDRandom = ssIDRandom + stringRandom;
    stringRandom = get_random_string(4).c_str();
    PASSRandom = PASSWORD;
    PASSRandom = PASSRandom + stringRandom;

    myoledviewwifiap = new MyOledViewWifiAp();
    myoledviewwifiap->setNomDuSysteme("SAC system");
    myoledviewwifiap->setNomDuSysteme("AP Configuration");
    myoledviewwifiap->setSsIdDuSysteme(String(ssIDRandom).c_str());
    myoledviewwifiap->setPassDuSysteme(String(PASSRandom).c_str()); 
    myoled->displayView(myoledviewwifiap);
   
    Serial.print("SSID : "); Serial.println(ssIDRandom);
    Serial.print("PASS : "); Serial.println(PASSRandom);

    char strToPrint[128];
    sprintf(strToPrint, "Identification : %s   MotDePasse: %s", ssIDRandom, PASSRandom);
    Serial.println(strToPrint);


    if (!wm.autoConnect(ssIDRandom.c_str(), PASSRandom.c_str()))
    {
        Serial.println("Erreur de connexion.");
    }
    else 
    {
        Serial.println("Connexion Établie.");

        //Faire clignoter les Leds à l'initialisation une fois la connexion établie
        for (int i=0;i<2;i++) 
        {
            digitalWrite(GPIO_PIN_LED_LOCK_ROUGE,HIGH);
            digitalWrite(GPIO_PIN_LED_HEAT_JAUNE,HIGH);
            digitalWrite(GPIO_PIN_LED_OK_VERTE,HIGH);
            delay(1000);
            
            digitalWrite(GPIO_PIN_LED_LOCK_ROUGE,LOW);
            digitalWrite(GPIO_PIN_LED_HEAT_JAUNE,LOW);
            digitalWrite(GPIO_PIN_LED_OK_VERTE,LOW);
            delay(1000); 
        }
        myoledviewworkingoff = new MyOledViewWorkingOFF();
        myoled->displayView(myoledviewworkingoff);
    }

    // ----------- Routes du serveur ----------------
    myServer = new MyServer(80);
    myServer->initAllRoutes();
    myServer->initCallback(&CallBackMessageListener);

    //Initiation pour la lecture de la température
    temperatureStub = new TemperatureStub;
    temperatureStub->init(DHTPIN, DHTTYPE); //Pin 15 et Type DHT22
 }

void loop() 
{
    //Gestion de la température
    float t = temperatureStub->getTemperature();
    tempDuFour = String(t).c_str();
    sprintf(buffer, "%4.1f", t);
    NouveauParametre();

    if(btnFour == true)
    {
        //Si la température est entre 20.7 et 25.5
        if(t >= (temperatureDemandee * 0.90) && t <= (temperatureDemandee / 0.90)) 
        {
            etatDuFour = 2;
            printf("Il vous reste %ld secondes.\n", nbrDeSecondes);
            nbrDeSecondes --;

            digitalWrite(GPIO_PIN_LED_OK_VERTE,LOW);
            digitalWrite(GPIO_PIN_LED_LOCK_ROUGE,HIGH);
            digitalWrite(GPIO_PIN_LED_HEAT_JAUNE,LOW);
                
            if(nbrDeSecondes == 0)
            {
                Serial.println("Fin du temps. Le séchage du bois est terminé !");
                digitalWrite(GPIO_PIN_LED_LOCK_ROUGE,LOW);
                digitalWrite(GPIO_PIN_LED_OK_VERTE,HIGH);
                etatDuFour = 0;
                btnFour = false;
                nbrDeSecondes = tempsDeSechage;
            }
        }
            //Si la température ambiante est inférieur à celle demandée
            if(t < (temperatureDemandee * 0.90))
            {
                    etatDuFour = 1;

                    digitalWrite(GPIO_PIN_LED_LOCK_ROUGE,LOW);
                    digitalWrite(GPIO_PIN_LED_HEAT_JAUNE,HIGH);

                    btnFour = false;
                    nbrDeSecondes = tempsDeSechage;
            }

            //Si la température ambiante est supérieur à celle demandée
            if(t > (temperatureDemandee / 0.90))
            {
                    etatDuFour = 1;
                    
                    digitalWrite(GPIO_PIN_LED_LOCK_ROUGE,LOW);
                    digitalWrite(GPIO_PIN_LED_HEAT_JAUNE,HIGH);
            }
    }
    delay(1000);
}