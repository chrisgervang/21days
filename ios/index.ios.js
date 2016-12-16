import React, { Component } from 'react';
import { access_token, device_id } from './token';
import {
  AppRegistry,
  StyleSheet,
  Text,
  TouchableHighlight,
  View
} from 'react-native';

export default class test extends Component {
    _onPressButton() {
      console.log("You tapped the button!")
      // completeHabit('brush')
    }


  render() {
    return (
      <View style={{flex: 1, marginTop: 60}}>
        <View style={{flexDirection: 'row'}}>
          <View style={{flex: 1, backgroundColor: 'rgba(0,0,0,0.05)', flexDirection: 'column', justifyContent: 'center', alignItems: 'center'}}>
            <TouchableHighlight onPress={this._onPressButton}>
              <View style={{height: 150, width: 150, padding: 25, backgroundColor: 'steelblue', margin: 5}}/>
            </TouchableHighlight>
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

AppRegistry.registerComponent('ios', () => test);

function completeHabit(habit) {
  fetch(`https://api.particle.io/v1/devices/${device_id}/habit`, {method: 'POST', headers: { "Content-Type": "application/x-www-form-urlencoded" }, body: `args=${habit}&access_token=${access_token}`})
}
