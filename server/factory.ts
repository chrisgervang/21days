import * as moment from 'moment-timezone';
import { History } from './commands';

interface GridHistory {
    "brush twice": boolean[]
    "dont murder": boolean[]
    "no sweets": boolean[]
    "workout": boolean[]
    "sleep by 12am": boolean[]
    "on time": boolean[]
}

export function makeHistory(rows: History[], timezone: string): GridHistory {
    const TwentyTwoDaysAgo = moment().tz(timezone).subtract(21, 'days');
    var habitsHistory: GridHistory = {
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

    return habitsHistory    
}

interface StringHistory {
    history: string
    order: string[]
}

export function gridToString(grid: GridHistory): StringHistory {
    var history = "";
    Object.keys(grid).forEach(function(habit, index, keys){
        const historyOfHabit = grid[habit].reduce((prev, curr) => {
            return prev += String(curr)
        }, "")
        console.log(historyOfHabit)
        history += `${historyOfHabit}${keys.length === index + 1 ? "" : ","}`
    })
    return {history: history, order: Object.keys(grid)};
}




