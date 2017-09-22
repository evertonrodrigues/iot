var coap = require('coap')
  , server = coap.createServer()

server.on('request', function (req, res) {
  req.payload = JSON.parse(String(req.payload));
  console.log(req.payload);
  res.end(JSON.stringify({ msg: 'Hello ' + req.payload.data.temp }));
})

// the default CoAP port is 5683
server.listen(function () {
  console.log('On!');

  //   var req = coap.request('coap://localhost/Matteo')

  //   req.on('response', function(res) {      
  //     console.log("res ", res);
  //     // res.pipe(process.stdout)
  //     // res.on('end', function() {
  //     //   process.exit(0)
  //     // })
  //   })

  //   req.end()
})
