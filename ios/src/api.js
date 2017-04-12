function parseText(response) {
  return response.text()
}

function parseJSON(response) {
  return response.json()
}

export function completeHabit(habit) {
  return fetch(`https://server-21days.herokuapp.com/device/track`, 
    {
      method: 'POST', 
      headers: { "Content-Type": "application/json" }, 
      body: JSON.stringify({
        "api_key": api_key,
        "coreid": device_id,
        "data": habit
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