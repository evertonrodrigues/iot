const coap = require('coap');

//const req = coap.request('coap://localhost/asdasdsad')

const req = coap.request('coap://10.101.42.94/light')

// const payload = { data: { temp: 12 } };
// req.write(JSON.stringify(payload));

req.write('0');

req.on('response', function (res) {
    res.pipe(process.stdout)
    res.on('end', function () {
        process.exit(0)
    })
});

req.end()


