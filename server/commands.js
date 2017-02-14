const ID_TO_HISTORY = 
`WITH owner AS (SELECT email AS email FROM public.user WHERE profile ->> 'deviceId' = $1::text)
SELECT habit AS habit, completed AS completed
FROM public.habit 
WHERE user_email = (SELECT email FROM owner) AND completed > current_date - INTERVAL '22 days'`

export function getHistory(client, coreid){
    return new Promise(function(resolve,reject){
        client.query(ID_TO_HISTORY, [coreid], function(err, result) {
             if(err !== null) return reject(err);
             resolve(result);
         });
    });
}

const ID_TO_TIMEZONE = 
`SELECT profile ->> 'timezone' as timezone FROM public.user WHERE profile ->> 'deviceId' = $1::text`

export function getTimezone(client, coreid){
    return new Promise(function(resolve,reject){
        client.query(ID_TO_TIMEZONE, [coreid], function(err, result) {
             if(err !== null) return reject(err);
             resolve(result);
         });
    });
}

const INSERT_HABIT = 
`WITH owner AS (SELECT email AS email FROM public.user WHERE profile ->> 'deviceId' = $2::text)
INSERT INTO public.habit (habit, user_email) 
VALUES ($1::text, (SELECT email FROM owner))`;

export function insertHabit(client, habit, coreid){
    return new Promise(function(resolve,reject){
        client.query(INSERT_HABIT, [habit, coreid], function(err, result) {
             if(err !== null) return reject(err);
             resolve(result);
         });
    });
}