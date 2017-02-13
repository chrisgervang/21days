var express = require('express');
var app = express();
var pg = require('pg');
var moment = require('moment-timezone');
var bodyParser = require('body-parser');

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

const ID_TO_HISTORY = 
`WITH owner AS (SELECT email AS email FROM public.user WHERE profile ->> 'deviceId' = $1::text)
SELECT habit AS habit, completed AS completed
FROM public.habit 
WHERE user_email = (SELECT email FROM owner) AND completed > current_date - INTERVAL '22 days'`

const ID_TO_TIMEZONE = 
`SELECT profile ->> 'timezone' as timezone FROM public.user WHERE profile ->> 'deviceId' = $1::text`

function isAuthenticated(request) {
    var key = process.env.API_KEY;
    return key === request.body.api_key
}

function makeHistory(rows, timezone) {
    const TwentyTwoDaysAgo = moment().tz(timezone).subtract(21, 'days');
    var habitsHistory ={
        "brush twice": [],
        "dont murder": [],
        "no sweets": [],
        "workout": [],
        "sleep by 12am": [],
        "on time": []
    }
    console.log(timezone);
    for (var day = 0; day < 22; day++) {
        var date = moment.tz(TwentyTwoDaysAgo, timezone).add(day, 'day');
        // Max length 6. PK guarentees only one of each type of habit comopleted on any day.
        var completed = rows.filter((row) => {
            if(row.habit === "dont murder") {
                //console.log(row.completed, date.toISOString(), moment.tz(row.completed, "Etc/UCT").isSame(date, 'day'))
            }
            return moment.tz(row.completed, timezone).isSame(date, 'day');
        });

        if(completed.length > 6) console.error("completed length is over 6: " + completed.length)
        for(var habit in habitsHistory) {
            var found = completed.find((val) => {
                return val.habit === habit
            })

            if(!!found) {
                console.log("completed", found.habit, found.completed)
                habitsHistory[habit].push(1)
            } else {
                habitsHistory[habit].push(0)
            }
        }      
    }
    console.log(JSON.stringify(habitsHistory))

    var history = "";
    Object.keys(habitsHistory).forEach(function(habit, index, keys){
        const historyOfHabit = habitsHistory[habit].reduce((prev, curr) => {
            return prev += String(curr)
        }, "")
        console.log(historyOfHabit)
        history += `${historyOfHabit}${keys.length === index + 1 ? "" : ","}`
    })
    return {history: history, order: Object.keys(habitsHistory)};
}

function getHistory(client, coreid){
    return new Promise(function(resolve,reject){
        client.query(ID_TO_HISTORY, [coreid], function(err, result) {
             if(err !== null) return reject(err);
             resolve(result);
         });
    });
}

function getTimezone(client, coreid){
    return new Promise(function(resolve,reject){
        client.query(ID_TO_TIMEZONE, [coreid], function(err, result) {
             if(err !== null) return reject(err);
             resolve(result);
         });
    });
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
})

const INSERT_HABIT = 
`WITH owner AS (SELECT email AS email FROM public.user WHERE profile ->> 'deviceId' = $2::text)
INSERT INTO public.habit (habit, user_email, completedDate) 
VALUES ($1::text, (SELECT email FROM owner), $3)`;

function insertHabit(client, coreid, date){
    return new Promise(function(resolve,reject){
        client.query(INSERT_HABIT, [habit, id, date], function(err, result) {
             if(err !== null) return reject(err);
             resolve(result);
         });
    });
}

app.post('/device/track', function (request, response) {
    if(isAuthenticated(request)) {
        var habit = request.body.data;
        var id = request.body.coreid;

        pg.connect(process.env.DATABASE_URL, function(err, client, done) {
            getTimezone(client, id).then((timezone) => {
                var date = moment().tz(timezone).format("YYYY-MM-DD");
                console.log(date);
                insertHabit(client, id, date).then((res) => {
                    done();
                    response.send(JSON.stringify(res));
                }, (err) => {
                    done();
                    response.send(err);
                })
            }, (err) => {
                done();
                response.send(err);
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