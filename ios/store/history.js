export const init = {
    "brush twice": [],
    "dont murder": [],
    "no sweets": [],
    "workout": [],
    "sleep by 12am": [],
    "on time": []
}

export default (state = init, action) => {
    switch(action.type) {
        case "load_history": 
            state = action.history
            return state
        default: 
            return state
    }
}

export const actions = {
    loadHistory: history => ({ type: "load_history", history })
}

export const selectors = {
    fromDaysAgo: (state, daysAgo) => (
        Object.keys(state.history).reduce((history, habit) => {
            const arr = state.history[habit]
            history[habit] = arr[arr.length - 1 - daysAgo] || false
            return history
        }, {})
    )
}