import * as Redux from 'redux'

import history from './history'

const reducers = Redux.combineReducers({
    history
})

export const createStore = (initialState) =>
    Redux.createStore(reducers, initialState)