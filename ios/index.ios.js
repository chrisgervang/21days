import React, { Component } from 'react';
import { access_token, device_id } from './token';
import {
  AppRegistry,
  StyleSheet,
  Text,
  View
} from 'react-native';

export default class AwesomeProject extends Component {
  render() {
    return (
      <View style={{flex: 1, marginTop: 60}}>
        <View style={{flexDirection: 'row'}}>
          <View style={{flex: 1, backgroundColor: 'rgba(0,0,0,0.05)', flexDirection: 'column', justifyContent: 'center', alignItems: 'center'}}>
            <View style={{height: 150, width: 150, padding: 25, backgroundColor: 'steelblue', margin: 5}} />
            <View style={{height: 150, width: 150, padding: 25, backgroundColor: 'steelblue', margin: 5}} />
            <View style={{height: 150, width: 150, padding: 25, backgroundColor: 'steelblue', margin: 5}} />
          </View>
          <View style={{flex: 1, backgroundColor: 'rgba(0,0,0,0.05)', flexDirection: 'column', justifyContent: 'center', alignItems: 'center'}}>
            <View style={{height: 150, width: 150, padding: 25, backgroundColor: 'steelblue', margin: 5}} />
            <View style={{height: 150, width: 150, padding: 25, backgroundColor: 'steelblue', margin: 5}} />
            <View style={{height: 150, width: 150, padding: 25, backgroundColor: 'steelblue', margin: 5}} />
          </View>
        </View>
      </View>
    );
  }
}

AppRegistry.registerComponent('AwesomeProject', () => AwesomeProject);

function completeHabit(habit) {
  fetch(`https://api.particle.io/v1/devices/${device_id}/habit`, {method: 'POST', headers: { "Content-Type": "application/x-www-form-urlencoded" }, body: `args=${habit}&access_token=${access_token}`})
}