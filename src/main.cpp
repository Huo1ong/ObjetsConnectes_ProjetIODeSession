/* Copyright (C) 2021 Alain Dube
 * All rights reserved.
 *
 * Projet Sac
 * Ecole du Web
 * Cours Objets connectés (c)2021
 *  
    @file     main.cpp
    @author   Alain Dubé
    @version  1.1 21/08/15 

    Historique des versions
           Version    Date       Auteur       Description
           1.1        21/08/15  Alain       Première version du logiciel

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

#include <iostream>
#include <string>

#include <Arduino.h>
#include <ArduinoJson.h>

#include "myFunctions.cpp" //fonctions utilitaires

using namespace std;

#include <HTTPClient.h>
#include <WiFiManager.h>
WiFiManager wm;
#define WEBSERVER_H

//Pour avoir les données du senseur de température
#include "TemperatureStub.h"
#define DHTPIN  15   // Pin utilisée par le senseur / DHT22
#define DHTTYPE DHT22  //Le type de senseur utilisé (mais ce serait mieux d'avoir des DHT22 pour plus de précision)
TemperatureStub *temperatureStub = NULL;

//Pour la gestion du serveur ESP32
#include "MyServer.h"
MyServer *myServer = NULL;

//Variable pour la connection Wifi
const char *SSID = "SAC_";
const char *PASSWORD = "sac_";
String ssIDRandom;

//LEDS 
//Définition des trois leds de statut
#define GPIO_PIN_LED_OK_GREEN           14 //GPIO14
#define GPIO_PIN_LED_HEAT_BLUE        27 //GPIO27
#define GPIO_PIN_LED_LOCK_ROUGE         12 //GPIO12

std::string tempDuFour = "22"; //Lire le senseur de température

boolean btnFour = false; //Bouton démarrage du four

int delayTimer = 0; //Pour compter le nombre de secondes depuis le début

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
        return(tempDuFour.c_str());
    }

    //Bouton "Démarrage du four"
    if (string(actionToDo.c_str()).compare(string("btnFour")) == 0)
    {
       btnFour = true;
       return "ok";
    }


    std::string result = "";
    return result;
    }

void setup() { 
    Serial.begin(9600);
    delay(100);

    //Initiation pour la lecture de la température
    temperatureStub = new TemperatureStub;
    temperatureStub->init(DHTPIN, DHTTYPE); //Pin 15 et Type DHT22

    //LEDS
    //Initialisation des LED statuts
    pinMode(GPIO_PIN_LED_OK_GREEN, OUTPUT);
    pinMode(GPIO_PIN_LED_HEAT_BLUE, OUTPUT);
    pinMode(GPIO_PIN_LED_LOCK_ROUGE, OUTPUT);

    //Faire clignoter les Leds à l'initialisation
    for (int i=0;i<2;i++) {
        digitalWrite(GPIO_PIN_LED_OK_GREEN,HIGH);
        digitalWrite(GPIO_PIN_LED_HEAT_BLUE,HIGH);
        digitalWrite(GPIO_PIN_LED_LOCK_ROUGE,HIGH);
        delay(1000);
        digitalWrite(GPIO_PIN_LED_OK_GREEN,LOW);
        digitalWrite(GPIO_PIN_LED_HEAT_BLUE,LOW);
        digitalWrite(GPIO_PIN_LED_LOCK_ROUGE,LOW);
        delay(1000); 
    }

    //Connection au WifiManager
    String ssIDRandom, PASSRandom;
    String stringRandom;
    stringRandom = get_random_string(4).c_str();
    ssIDRandom = SSID;
    ssIDRandom = ssIDRandom + stringRandom;
    stringRandom = get_random_string(4).c_str();
    PASSRandom = PASSWORD;
    PASSRandom = PASSRandom + stringRandom;

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
    }

    // ----------- Routes du serveur ----------------
    myServer = new MyServer(80);
    myServer->initAllRoutes();
    myServer->initCallback(&CallBackMessageListener);
 }

void loop() 
{
    //Gestion de la température
    float t = temperatureStub->getTemperature();
    //Pas l'utilité pour le moment
        //Serial.print("Température : ");
        //Serial.println(t);
    tempDuFour = String(t).c_str();

    if(btnFour == true)
    {
        btnFour = false;
        Serial.println("Démarrage du four.");
        int i;
        for(i = 20; i != -1; i--)
        {
            printf("Il vous reste %ld secondes.\n", i);
            
        }
        printf("Fin du temps. Le séchage du bois est terminé !");
    }
    delay(1000);
}