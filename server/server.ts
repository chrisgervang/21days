import * as express from 'express';
import * as pg from 'pg';
import * as bodyParser from 'body-parser';
import * as WebRoutes from './web'
import * as DeviceRoutes from './device'
export var app = express();

export const habits = ["brush twice", "dont murder", "no sweets", "workout", "sleep by 12am", "on time"]

app.use(bodyParser.json({limit: "5mb"}));

// Use: https://server-21days.herokuapp.com/habits?user=chris@gervang.com&key=****
app.get('/history', function (request, response) {
    var key = process.env.API_KEY;
    if(key === request.query.key) {
        var user = request.query.user;
        var query = "SELECT habit, completed FROM public.habit WHERE user_email = '" + user + "'";
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

WebRoutes.history(app)
DeviceRoutes.history(app)
DeviceRoutes.track(app)

app.set('port', (process.env.PORT || 5000));

// app.use(express.static(__dirname + '/public'));

app.get('/', function(request, response) {
  response.send('pages/index');
});

app.listen(app.get('port'), function() {
  console.log('Node app is running on port', app.get('port'));
});