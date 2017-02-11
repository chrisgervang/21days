var express = require('express');
var app = express();
var pg = require('pg');

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

const habits = ["brush twice", "dont murder", "no sweets", "workout", "sleep by 12am", "on time"]


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

const ID_TO_USER = `SELECT email AS email FROM public.user WHERE profile ->> 'deviceId' = $1::text`

app.get('/device/history', function (request, response) {
    var key = process.env.API_KEY;
    //console.log(key, request.headers)
    if(key === request.headers.api_key) {
        pg.connect(process.env.DATABASE_URL, function(err, client, done) {
            client.query(ID_TO_USER, [request.headers.coreid], function(err, result) {
                done();
                if(err) {
                    console.error(err);
                    response.send("Error " + err);
                } else {
                    if(result.rows.length === 1) {
                        response.json(result.rows[0].email);
                    } else {
                        response.json(result)
                    }
                }
            })
        })



    } else {
        console.error("Auth no good");
        response.send("Error: Auth no good");
    }
})

app.get('/track', function (request, response) {
    var id = process.env.PARTICLE_ID;
    if(id === request.query.id) {
        var habit = request.query.habit;
        var user = request.query.user;
        var query = "INSERT INTO public.habit (habit, user_email) VALUES ('" + habit + "', '" + user + "')";
        pg.connect(process.env.DATABASE_URL, function(err, client, done) {
            client.query(query, function(err, result) {
                done();
                if(err) {
                    console.error(err);
                    response.send("Error " + err);
                } else {
                    response.send("Success " + result);
                }
            })
        })
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