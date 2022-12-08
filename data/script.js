function getFromESP_getNom () 
{
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function ()
    {
        if (this.readyState == 4 && this.status == 200)
        {
            document.getElementById("nom").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "getNomEsp", true);
    xhttp.send();
}

function getFromESP_getColorBackground () 
{
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function ()
    {
        if (this.readyState == 4 && this.status == 200)
        {
            document.getElementById("background").style.backgroundColor = "darkslategray";
        }
    };
    xhttp.open("GET", "getColorBackground", true);
    xhttp.send();
}


setInterval (function getNomSysteme()
{
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function()
    {
        if(this.status == 200)
        {
            var maReponseText = this.responseText.split(";");
            document.getElementById("temp").innerHTML = maReponseText[0];
            document.getElementById("secondes").innerHTML = maReponseText[1];
        }
    };
    xhttp.open("GET", "lireTemp", true);
    xhttp.send();
}, 1000);

function doAction(actionToDo) {
    var params = String("btnFour") + String("=") + String(actionToDo);
    var xhttp = new XMLHttpRequest();
    xhttp.open("POST", "btnFour", true);
    xhttp.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
    xhttp.send(params);
    demarrer = true;
}


function getFromESP_ListeWood(){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
        var reponse = this.responseText;
        var reponse = reponse.slice(0, -1);
        var lesBois = reponse.split(";");
        var x = document.getElementById("NomBois");

        for (let i = 0; i < lesBois.length; i++) {
          var option = document.createElement("option");
          option.text = lesBois[i];
          x.add(option);
          }
    }
    };
    xhttp.open("GET", "getListeWood", true);
    xhttp.send();
}

function afficherBois(){
    nomBois = document.getElementById("NomBois").value;
    var params = String("nomBois") + String("=") + String(nomBois);
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
        var reponse = this.responseText;
        var reponse = reponse.slice(0, -1);
        alert(reponse);
        var lesBois = reponse.split(";");
        document.getElementById("caracteristique_bois").innerHTML = lesBois[0];
        document.getElementById("controle_bois").innerHTML = lesBois[0];
        document.getElementById("caracteristique_type").innerHTML = lesBois[1];
        document.getElementById("caracteristique_origine").innerHTML = lesBois[2];
        document.getElementById("caracteristique_tempsDeSechage").innerHTML = lesBois[3];
        document.getElementById("tempsDeSechage").innerHTML = lesBois[3];
        document.getElementById("caracteristique_temperatureMin").innerHTML = lesBois[4];
        document.getElementById("controle_temperatureMin").innerHTML = lesBois[4];

    }
    };
    xhttp.open("POST", "afficherBois", true);
    xhttp.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
    xhttp.send(params);
}
   