var express = require('express');
var app = express();
var pg = require('pg');
var moment = require('moment-timezone');
var bodyParser = require('body-parser');
var rp = require('request-promise-native');

// app.get('/db', function (request, response) {
//   pg.connect(process.env.DATABASE_URL, function(err, client, done) {
//     client.query('SELECT * FROM test_table', function(err, result) {
//       done();
//       if (err)
//        { console.error(err); response.send("Error " + err); }
//       else
//        { response.json({results: result.rows} ); }
//     });
//   });
// });

function completeHabit(habit, coreid, accessToken) {
  return rp(`https://api.particle.io/v1/devices/${coreid}/habit`, {method: 'POST', headers: { "Content-Type": "application/x-www-form-urlencoded" }, body: `args=${habit}&access_token=${accessToken}`})
}

const habits = ["brush twice", "dont murder", "no sweets", "workout", "sleep by 12am", "on time"]

app.use(bodyParser.json({limit: "5mb"}));

// Use: https://server-21days.herokuapp.com/habits?user=chris@gervang.com&key=****
app.get('/habits', function (request, response) {
    var key = process.env.API_KEY;
    if(key === request.query.key) {
        var user = request.query.user;
        var query = "SELECT habit AS habit, completedDate AS completedDate FROM public.habit WHERE user_email = '" + user + "'";
        pg.connect(process.env.DATABASE_URL, function(err, client, done) {
            client.query(query, function(err, result) {
                done();
                if(err) {
                    console.error(err);
                    response.send("Error " + err);
                } else {
                    response.json(result.rows);
                }
            })
        })
    } else {
        console.error("Auth did not match");
        response.send("Error Auth did not match");
    }
})

function isAuthenticated(request) {
    var key = process.env.API_KEY;
    return key === request.body.api_key
}



app.post('/device/history', function (request, response) {
    if(isAuthenticated(request)) {
        pg.connect(process.env.DATABASE_URL, function(err, client, done) {
            Promise.all([getHistory(client, request.body.coreid), getTimezone(client, request.body.coreid)])
            .then((results)=> {
                done();
                console.log(results[0].rows);
                response.send(JSON.stringify(makeHistory(results[0].rows, results[1].rows[0].timezone)));
            }, (err) => {
                done();
                response.send(err);
            })
        });
    } else {
        console.error("Auth no good");
        response.send("Error: Auth no good");
    }
});

app.post('/device/track', function (request, response) {
    if(isAuthenticated(request)) {
        var habit = request.body.data;
        var coreid = request.body.coreid;

        pg.connect(process.env.DATABASE_URL, function(err, client, done) {
            Promise.all([getHistory(client, coreid), getTimezone(client, coreid)])
            .then((results)=> {
                var timezone = results[1].rows[0].timezone;
                console.log(timezone);

                const doneForDay = isDoneForDay(results[0].rows, timezone);

                if(doneForDay) {
                    done();
                    response.send("Already Done For Today");
                } else {
                    console.log("Inserting. Completed another habit!");
                    Promise.all([insertHabit(client, habit, coreid), completeHabit(habit, coreid, "7bc0a91537202df9bc55760e960fa05b6f84163c")]).then((res) => {
                        done();
                        console.log(res);
                        
                        response.send(JSON.stringify(res));
                    }, (err) => {
                        done();
                        response.send(err);
                    });
                }
                
            }, (err) => {
                done();
                response.send(err);
            });
        });
    } else {
        console.error("Auth did not match");
        response.send("Error Auth did not match");
    }
    
})

app.set('port', (process.env.PORT || 5000));

// app.use(express.static(__dirname + '/public'));

// views is directory for all template files
// app.set('views', __dirname + '/views');
// app.set('view engine', 'ejs');

app.get('/', function(request, response) {
  response.send('pages/index');
});

app.listen(app.get('port'), function() {
  console.log('Node app is running on port', app.get('port'));
});