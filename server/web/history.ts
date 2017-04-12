import * as pg from 'pg';
import { Express } from 'express'
import { isAuthenticated } from '../helpers'
import { getHistory, getTimezone, insertHabit } from '../commands'
import { makeHistory, gridToString } from '../factory';

interface Body {
    coreid: string
    api_key: string
}

export default function init(app: Express) {
    app.post('/web/history', function (request, response) {
        if(isAuthenticated(request)) {
            const body = request.body as Body
            pg.connect(process.env.DATABASE_URL, function(err, client, done) {
                Promise.all([getHistory(client, body.coreid), getTimezone(client, body.coreid)])
                .then(([history, timezone])=> {
                    done();
                    console.log(history);
                    response.send(JSON.stringify(makeHistory(history, timezone)));
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
}
