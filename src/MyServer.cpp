/**
    Gestion d'un serveur WEB
    @file MyServer.cpp
    @author Alain Dubé
    @version 1.1 20/11/20 
*/
#include <Arduino.h>
#include "MyServer.h"
using namespace std;

typedef std::string (*CallbackType)(std::string);
CallbackType MyServer::ptrToCallBackFunction = NULL;

//Exemple pour appeler une fonction CallBack
//if (ptrToCallBackFunction) (*ptrToCallBackFunction)(stringToSend); 
void MyServer::initCallback(CallbackType callback) {
    ptrToCallBackFunction = callback;
    }

void MyServer::initAllRoutes() { 
    currentTemperature = 3.3f;

    //Initialisation du SPIFF.
    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
        }

    //Route initiale (page html)
    this->on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(SPIFFS, "/index.html", "text/html");
    });

    this->on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(SPIFFS, "/index.html", "text/html");
    });

    //Route du script JavaScript
    this->on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/script.js", "text/javascript");
        });
    
    //CSS
    this->on("/index.css", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(SPIFFS, "/index.css", "text/css");
    });
    
    //Images
    this->on("/sac.png", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(SPIFFS, "/sac.png", "image/png");
    });
    
    //Bouton
    this->on("/start", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        if (ptrToCallBackFunction) {(*ptrToCallBackFunction)("button start");
        }
        //request->send(Serial.println("START"));
    });

    //Bouton "Démarrage du four" -> compte à rebours
    this->on("/btnFour", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (request->hasParam("btnFour", true)) {
        String actionToDo = request->getParam("btnFour", true)->value();
        if (string(actionToDo.c_str()).compare(string("btnFour")) == 0) {
            if (ptrToCallBackFunction)(*ptrToCallBackFunction)("btnFour");
            }
        }
        request->send(204);
        });
    
    //getNom
    this->on("/getNomEsp", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        std::string repString = "";
        if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)("askNomFour");
        String lireNomDuFour =String(repString.c_str());
        request->send(200, "text/plain", lireNomDuFour );
    });

    //getTemperature
    this->on("/lireTemp", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        std::string repString = "";
        if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)("askTempFour");
        String lireTempDuFour =String(repString.c_str());
        request->send(200, "text/plain", lireTempDuFour);
    });


    this->onNotFound([](AsyncWebServerRequest *request)
    {
        request->send(404, "text/plain", "Page non fonctionnelle.");
    });
    
    this->begin();
};