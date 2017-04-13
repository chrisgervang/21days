import { api_key, device_id } from '../token';

function parseText(response) {
  return response.text()
}

function parseJSON(response) {
  return response.json()
}

export function trackHabit(habit, daysAgo, done) {
  return fetch(`https://server-21days.herokuapp.com/web/track`, 
    {
      method: 'POST', 
      headers: { "Content-Type": "application/json" }, 
      body: JSON.stringify({
        "api_key": api_key,
        "coreid": device_id,
        "data": habit,
        "days_ago": daysAgo,
        "done": done
      })
    }
  ).then((res) => {
      return parseText(res)
  })
}

export function getHistory() {
  return fetch(`https://server-21days.herokuapp.com/web/history`, 
    {
      method: 'POST', 
      headers: { "Content-Type": "application/json" }, 
      body: JSON.stringify({
        "api_key": api_key,
        "coreid": device_id
      })
    }
  ).then((res) => {
      return parseJSON(res)
  })
}