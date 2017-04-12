import * as rp from 'request-promise-native';
import * as moment from 'moment-timezone';
import { History } from './commands';

export function isAuthenticated(request) {
    var key = process.env.API_KEY;
    return key === request.body.api_key
}

export function isDoneForDay(history: History[], referenceDate: string, timezone: string, habit: string) {
    var date = moment(referenceDate).tz(timezone);
    //, moment().tz('Etc/UTC').format("YYYY-MM-DD")
    console.log(date.format());
    
    var doneForDay = history.some((row) => {
        if(row.habit === habit && moment.tz(row.completed, timezone).isSame(date, 'day')) {
            console.log(row, habit);
            console.log(moment.tz(row.completed, timezone).format(), moment.tz(row.completed, timezone).isSame(date, 'day'))
        }
        return row.habit === habit && moment.tz(row.completed, timezone).isSame(date, 'day');
    });

    return doneForDay;
}

export function completeHabit(habit:string, coreid: string, accessToken: string) {
  return rp(`https://api.particle.io/v1/devices/${coreid}/habit`, 
    {
        method: 'POST', 
        headers: { "Content-Type": "application/x-www-form-urlencoded" }, 
        body: `args=${habit}&access_token=${accessToken}`
    })
}