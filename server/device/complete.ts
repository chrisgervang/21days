import * as pg from 'pg';
import * as moment from 'moment-timezone';

import { Express } from 'express'
import { isAuthenticated, completeHabit, isDoneForDay } from '../helpers'
import { getHistory, getTimezone, insertHabit } from '../commands'
import { makeHistory } from '../factory';

interface Body {
    coreid: string
    api_key: string
    habit: string
}

export default function init(app: Express) {
    app.post('/device/complete', function (request, response) {
        if(isAuthenticated(request)) {
            const body = request.body as Body
            const habit = body.habit;
            const coreid = body.coreid;

            pg.connect(process.env.DATABASE_URL, function(err, client, done) {
                Promise.all([getHistory(client, coreid), getTimezone(client, coreid)])
                .then(([history, timezone])=> {
                    console.log(timezone, history);

                    const doneForDay = isDoneForDay(history, moment().utc().format(), timezone, habit);

                    if(doneForDay) {
                        done();
                        response.send("Already Done For Today");
                    } else {
                        console.log("Inserting. Completed another habit!");
                        Promise.all([insertHabit(client, habit, coreid), completeHabit(habit, coreid, process.env.ACCESS_TOKEN)]).then((res) => {
                            done();
                            console.log(res);
                            
                            response.send(JSON.stringify(res));
                        }, (err) => {
                            done();
                            response.status(500)
                            response.send(err);
                        });
                    }
                    
                }, (err) => {
                    done();
                    response.status(500)
                    response.send(err);
                });
            });
        } else {
            console.error("Auth did not match");
            response.status(401)
            response.send("Error Auth did not match");
        }
        
    })
}