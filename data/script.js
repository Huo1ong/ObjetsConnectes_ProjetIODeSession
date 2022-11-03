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
            document.getElementById("temp").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "lireTemp", true);
    xhttp.send();
}, 5000);
   