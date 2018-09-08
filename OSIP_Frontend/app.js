var finalhandler = require('finalhandler')
var http = require('http');
var fs = require('fs');
var express = require('express');
var ss = require('serve-static');

var app = express();

var serve = ss('public', {'index': ['index.html', 'index.htm']});

var server = http.createServer(function onRequest (req, res) {
    serve(req, res, finalhandler(req, res))
});

server.listen(8080);