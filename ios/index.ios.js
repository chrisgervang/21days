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
    _onPressButtonBrush() {
      console.log("BRUSH")
      // completeHabit('brush')
      testLog();
    }

    _onPressButtonTime() {
      console.log("TIME")
      // completeHabit('time')
    }

    _onPressButtonMurder() {
      console.log("MURDER")
      // completeHabit('murder')
    }

    _onPressButtonSleep() {
      console.log("SLEEP")
      // completeHabit('time')
    }

    _onPressButtonWorkout() {
      console.log("WORKOUT")
      // completeHabit('time')
    }

    _onPressButtonSweets() {
      console.log("SWEETS")
      // completeHabit('time')
    }


  render() {
    return (
      <View style={{flex: 1, marginTop: 60}}>
        <View style={{flexDirection: 'row'}}>
          <View style={{flex: 1, backgroundColor: 'rgba(0,0,0,0.05)', flexDirection: 'column', justifyContent: 'center', alignItems: 'center'}}>
            <TouchableHighlight onPress={this._onPressButtonBrush}>
              <View style={{height: 150, width: 150, padding: 25, backgroundColor: 'steelblue', margin: 5}}/>
            </TouchableHighlight>
            <TouchableHighlight onPress={this._onPressButtonTime}>
              <View style={{height: 150, width: 150, padding: 25, backgroundColor: 'steelblue', margin: 5}}/>
            </TouchableHighlight>
            <TouchableHighlight onPress={this._onPressButtonMurder}>
              <View style={{height: 150, width: 150, padding: 25, backgroundColor: 'steelblue', margin: 5}}/>
            </TouchableHighlight>
          </View>
          <View style={{flex: 1, backgroundColor: 'rgba(0,0,0,0.05)', flexDirection: 'column', justifyContent: 'center', alignItems: 'center'}}>
            <TouchableHighlight onPress={this._onPressButtonSleep}>
              <View style={{height: 150, width: 150, padding: 25, backgroundColor: 'steelblue', margin: 5}}/>
            </TouchableHighlight>
            <TouchableHighlight onPress={this._onPressButtonWorkout}>
              <View style={{height: 150, width: 150, padding: 25, backgroundColor: 'steelblue', margin: 5}}/>
            </TouchableHighlight>
            <TouchableHighlight onPress={this._onPressButtonSweets}>
              <View style={{height: 150, width: 150, padding: 25, backgroundColor: 'steelblue', margin: 5}}/>
            </TouchableHighlight>
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

function testLog() {
  console.log('testLog works');
}
