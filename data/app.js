const http = require('http')
const fs = require('fs')
const port = 3000

const server = http.createServer(function(req, res) {
    res.writeHead(200, { 'Content-Type': 'text/html' })
    fs.readFile('index.html', function(error, data) {
        if(error)
        {
            res.writeHead(404)
            res.write('Erreur : Fichier non trouvé !')
        }
        else
        {
            res.write(data)
        }
        res.end
    })     
})

server.listen(port, function (error) {
    if(error)
    {
        console.log('Quelque chose ne fonctionne pas très bien.')
    }
    else
    {
        console.log('Le server fonctionne sur le port ' + port)
    }
})