import React, { Component } from 'react';
import { Track } from './src/Track'

import {
  AppRegistry,
  StyleSheet,
  View, 
  StatusBar,
} from 'react-native';

export default class Main extends Component { 
  render() {
    return (
      <View style={{flex: 1, backgroundColor: 'black'}}>
        <StatusBar
          barStyle='light-content'
        />
        <View style={{marginTop: 24, flex: 1}}>
          <Track/>
        </View>
      </View>
    )
  }
}

AppRegistry.registerComponent('days', () => Main);

function testLog() {
  console.log('testLog works');
}
