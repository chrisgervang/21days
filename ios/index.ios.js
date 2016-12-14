/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 * @flow
 */

import React, { Component } from 'react';
import { access_token, device_id } from './token';
import {
  AppRegistry,
  StyleSheet,
  Text,
  View
} from 'react-native';

export default class ios extends Component {
  render() {
    return (
      <View style={styles.container}>
        <Text style={styles.welcome}>
          Welcome to React Native!
        </Text>
        <Text style={styles.instructions}>
          To get started, edit index.ios.js
        </Text>
        <Text style={styles.instructions}>
          Press Cmd+R to reload,{'\n'}
          Cmd+D or shake for dev menu
        </Text>
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#F5FCFF',
  },
  welcome: {
    fontSize: 20,
    textAlign: 'center',
    margin: 10,
  },
  instructions: {
    textAlign: 'center',
    color: '#333333',
    marginBottom: 5,
  },
});

function completeHabit(habit) {
  fetch(`https://api.particle.io/v1/devices/${device_id}/habit`, {method: 'POST', headers: { "Content-Type": "application/x-www-form-urlencoded" }, body: `args=${habit}&access_token=${access_token}`})
}

AppRegistry.registerComponent('ios', () => ios);
