import React, { Component } from 'react';
import { access_token, device_id } from './token';
import {
  AppRegistry,
  StyleSheet,
  Text,
  TouchableHighlight,
  View, 
  Image,
  Dimensions,
  StatusBar
} from 'react-native';

export default class test extends Component {
    _onPressButtonBrush() {
      console.log("BRUSH")
      completeHabit('brush')
      testLog();
    }

    _onPressButtonTime() {
      console.log("TIME")
     completeHabit('onTime')
    }

    _onPressButtonMurder() {
      console.log("MURDER")
      completeHabit('murder')
    }

    _onPressButtonSleep() {
      console.log("SLEEP")
      completeHabit('sleep')
    }

    _onPressButtonWorkout() {
      console.log("WORKOUT")
      completeHabit('workout')
    }

    _onPressButtonSweets() {
      console.log("SWEETS")
      completeHabit('sweets')
    }


  render() {
    var deviceWidth = Dimensions.get('window').width;
    const styles = {
      image: {
        resizeMode: 'contain',
        width: deviceWidth / 2,
        height: deviceWidth / 2
      }
    }
    return (
      <View style={{flex: 1, backgroundColor: 'black'}}>
        <StatusBar
          barStyle='light-content'
        />
        <View style={{flex: 1, flexDirection: 'row'}}>
            <View style={{flex: 1, flexDirection: 'column', justifyContent: 'center', alignItems: 'center'}}>
                <TouchableHighlight onPress={this._onPressButtonBrush}>
                  <Image style={styles.image} source={require('./img/brush.jpg')} />
                </TouchableHighlight>
                <TouchableHighlight onPress={this._onPressButtonTime}>
                  <Image style={styles.image} source={require('./img/onTime.jpg')}/>
                </TouchableHighlight>
                <TouchableHighlight onPress={this._onPressButtonMurder}>
                  <Image style={styles.image} source={require('./img/murder.jpg')}/>
                </TouchableHighlight>
            </View>
            <View style={{flex: 1, flexDirection: 'column', justifyContent: 'center', alignItems: 'center'}}>
                <TouchableHighlight onPress={this._onPressButtonSleep}>
                  <Image style={styles.image} source={require('./img/sleep.jpg')}/>
                </TouchableHighlight>
                <TouchableHighlight onPress={this._onPressButtonWorkout}>
                  <Image style={styles.image} source={require('./img/workout.jpg')}/>
                </TouchableHighlight>
                <TouchableHighlight onPress={this._onPressButtonSweets}>
                  <Image style={styles.image} source={require('./img/sweets.jpg')}/>
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
