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

app.get('/habits', function (request, response) {
    var id = process.env.PARTICLE_ID;
    if(id === request.query.id) {
        var user = request.query.user;
        var query = "SELECT (habit, completedDate) FROM public.habit WHERE user_email = '" + user + "'";
        pg.connect(process.env.DATABASE_URL, function(err, client, done) {
            client.query(query, function(err, result) {
                done();
                if(err) {
                    console.error(err);
                    response.send("Error " + err);
                } else {
                    response.json(result);
                }
            })
        })
    } else {
        console.error("Auth did not match");
        response.send("Error Auth did not match");
    }
})

app.get('/track', function (request, response) {
    var id = process.evn.PARTICLE_ID;
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