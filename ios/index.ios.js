import React, { Component } from 'react';
import { api_key, device_id } from './token';
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
    handlePressButtonBrush() {
      console.log("BRUSH")
      completeHabit('brush twice')
    }

    handlePressButtonTime() {
      console.log("TIME")
     completeHabit('on time')
    }

    handlePressButtonMurder() {
      console.log("MURDER")
      completeHabit('dont murder')
    }

    handlePressButtonSleep() {
      console.log("SLEEP")
      completeHabit('sleep by 12am')
    }

    handlePressButtonWorkout() {
      console.log("WORKOUT")
      completeHabit('workout')
    }

    handlePressButtonSweets() {
      console.log("SWEETS")
      completeHabit('no sweets')
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
                <TouchableHighlight onPress={this.handlePressButtonBrush}>
                  <Image style={styles.image} source={require('./img/brush.jpg')} />
                </TouchableHighlight>
                <TouchableHighlight onPress={this.handlePressButtonTime}>
                  <Image style={styles.image} source={require('./img/onTime.jpg')}/>
                </TouchableHighlight>
                <TouchableHighlight onPress={this.handlePressButtonMurder}>
                  <Image style={styles.image} source={require('./img/murder.jpg')}/>
                </TouchableHighlight>
            </View>
            <View style={{flex: 1, flexDirection: 'column', justifyContent: 'center', alignItems: 'center'}}>
                <TouchableHighlight onPress={this.handlePressButtonSleep}>
                  <Image style={styles.image} source={require('./img/sleep.jpg')}/>
                </TouchableHighlight>
                <TouchableHighlight onPress={this.handlePressButtonWorkout}>
                  <Image style={styles.image} source={require('./img/workout.jpg')}/>
                </TouchableHighlight>
                <TouchableHighlight onPress={this.handlePressButtonSweets}>
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
  fetch(`https://server-21days.herokuapp.com/device/track`, 
    {
      method: 'POST', 
      headers: { "Content-Type": "application/json" }, 
      body: JSON.stringify({
        "api_key": api_key,
        "coreid": device_id,
        "data": habit
      })
    }
  )
}

function testLog() {
  console.log('testLog works');
}
