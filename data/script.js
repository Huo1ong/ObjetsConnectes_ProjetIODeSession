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
   