// Servidor ruta 3000
var express = require('express');
var app = express();

app.use(express.static('public'));

app.get('/', function (req, rep) {
  rep.sendFile(__dirname + '/database.html');
})

app.use(express.urlencoded({
  extended: false
}))

app.post('/query', (req, res) => {
  const query = req.body.query
  console.log(query)
  res.end()
})

app.post('/extendibleHashing', (req, res) => {
  const addon2 = require('./build/Release/calculate');
  let insert2 = addon2.selectAll();
  let separator = insert2.split('|');
  let finalSeparator = separator[0].split(',');
  let div = '<tr>< th scope = "row" > 1</th><td>Mark</td><td>Otto</td><td>@mdo</td></tr >';
  div = div.replace('Mark', finalSeparator[0]);
  div = div.replace('Otto', finalSeparator[1]);
  div = div.replace('@mdo', finalSeparator[2]);
  console.log(finalSeparator);
  console.log(div);


  res.redirect('/extendibleHashing.html')
})

app.post('/sequentialFile', (req, res) => {
  const addon2 = require('./build/Release/calculate');
  let insert2 = addon2.selectAll();
  let separator = insert2.split('|');
  let finalSeparator = separator[0].split(',');
  let div = '<tr>< th scope = "row" > 1</th><td>Mark</td><td>Otto</td><td>@mdo</td></tr >';
  div = div.replace('Mark', finalSeparator[0]);
  div = div.replace('Otto', finalSeparator[1]);
  div = div.replace('@mdo', finalSeparator[2]);
  console.log(finalSeparator);
  console.log(div);


  res.redirect('/sequentialFile.html')
})

app.listen(3000, function () {
  console.log("our server is live on port 3000");
})
// -------------------------------------------



// addons recuperamos las respuestas de c++
const addon = require('./build/Release/calculate');

const obj1 = addon.expor();
let insert = addon.insert();
const initArray = ["nombres", "Apellidos", "Edad"];
var otherArray = [];

// para separar el string y enviarlo a un array

let string = "";
for (let i = 0; i < obj1.length; i++) {
  if (obj1[i] == ' ') {
    otherArray.push(string);
    string = "";
  }
  string += obj1[i];
}
otherArray.push(string);

//

console.log(obj1);
console.time('c++');
//iria aqui el c++
console.timeEnd('c++');
console.log(insert);
console.log(otherArray);
// Prints: 'hello world

// -------------------------------