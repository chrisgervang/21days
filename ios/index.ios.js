import React, { Component } from 'react';
import Track from './src/Track'
import { createStore } from './store'
import { Provider } from 'react-redux'
import {
  AppRegistry,
  StyleSheet,
  View, 
  StatusBar,
} from 'react-native';

const store = createStore()

export default class Main extends Component { 
  render() {
    return (
      <Provider store={store}>
        <View style={{flex: 1, backgroundColor: 'black'}}>
          <StatusBar
            barStyle='light-content'
          />
          <View style={{marginTop: 24, flex: 1}}>
            <Track/>
          </View>
        </View>
      </Provider>
    )
  }
}

AppRegistry.registerComponent('days', () => Main);

function testLog() {
  console.log('testLog works');
}
