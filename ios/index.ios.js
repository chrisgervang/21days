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
  StatusBar,
  LayoutAnimation
} from 'react-native';

function parseText(response) {
  return response.text()
}

export default class test extends Component {
  constructor(props) {
    super(props)
    this.state = {
      brush: false,
      onTime: false,
      sleep: false,
      workout: false,
      noSweets: false,
      dontMurder: false
    }
  }

  handlePressButtonBrush = () => {
    console.log("BRUSH")
    completeHabit('brush twice').then((res) => {
      parseText(res).then(text => {
        if(text === "Already Done For Today") {        
          LayoutAnimation.spring();
          this.setState({brush: true});
        }
      })
    })
    LayoutAnimation.spring();
    this.setState({brush: true});
  }

  handlePressButtonTime = () => {
    console.log("TIME")
    completeHabit('on time').then((res) => {
      parseText(res).then(text => {
        if(text === "Already Done For Today") {        
          LayoutAnimation.spring();
          this.setState({onTime: true});
        }
      })
    })
    LayoutAnimation.spring();
    this.setState({onTime: true});
  }

  handlePressButtonMurder = () => {
    console.log("MURDER")
    completeHabit('dont murder').then((res) => {
      parseText(res).then(text => {
        if(text === "Already Done For Today") {
          LayoutAnimation.spring();
          this.setState({dontMurder: true});
        }
      })
    })
    LayoutAnimation.spring();
    this.setState({dontMurder: true});
  }

  handlePressButtonSleep = () => {
    console.log("SLEEP")
    completeHabit('sleep by 12am').then((res) => {
      parseText(res).then(text => {
        if(text === "Already Done For Today") {
          this.setState({sleep: true});
        }
      })
    })
    LayoutAnimation.spring();
    this.setState({sleep: true});
  }

  handlePressButtonWorkout = () => {
    console.log("WORKOUT")
    completeHabit('workout').then((res) => {
      parseText(res).then(text => {
        if(text === "Already Done For Today") {
          LayoutAnimation.spring();
          this.setState({workout: true});
        }
      })
    })
    LayoutAnimation.spring();
    this.setState({workout: true});
  }

  handlePressButtonSweets = () => {
    console.log("SWEETS")
    completeHabit('no sweets').then((res) => {
      parseText(res).then(text => {
        console.log(text)
        if(text === "Already Done For Today") {
          LayoutAnimation.spring();
          this.setState({noSweets: true});
        }
      })
    })
    LayoutAnimation.spring();
    this.setState({noSweets: true});
  }


  render() {
    
    return (
      <View style={{flex: 1, backgroundColor: 'black'}}>
        <StatusBar
          barStyle='light-content'
        />
        <View style={{flex: 1, flexDirection: 'row'}}>
            <View style={{flex: 1, flexDirection: 'column', justifyContent: 'center', alignItems: 'center'}}>
                <Button onPress={this.handlePressButtonBrush} img={require('./img/brush.jpg')} pressed={this.state.brush}
                  backgroundColor="#8A7EA8" activeColor="rgb(144, 0, 255)"/>
                <Button onPress={this.handlePressButtonTime} img={require('./img/onTime.jpg')} pressed={this.state.onTime}
                  backgroundColor="#E5CBA8" /*FFDEB3*/ activeColor="rgb(255, 255, 0)"/>
                <Button onPress={this.handlePressButtonMurder} img={require('./img/murder.jpg')} pressed={this.state.dontMurder}
                  backgroundColor="#0E1019" activeColor="rgb(255, 0, 25)"/>
            </View>
            <View style={{flex: 1, flexDirection: 'column', justifyContent: 'center', alignItems: 'center'}}>
                <Button onPress={this.handlePressButtonSleep} img={require('./img/sleep.jpg')} pressed={this.state.sleep}
                  backgroundColor=/*"#17344C"*/ "#081119" activeColor="rgb(0, 158, 255)"/>
                <Button onPress={this.handlePressButtonWorkout} img={require('./img/workout.jpg')} pressed={this.state.workout}
                  backgroundColor="#325866" activeColor="rgb(255, 127, 0)"/>
                <Button onPress={this.handlePressButtonSweets} img={require('./img/sweets.jpg')} pressed={this.state.noSweets}
                  backgroundColor="#74A574" activeColor="rgb(0, 255, 0)"/>
            </View>
        </View>
      </View>
    );
  }
}

class Button extends Component {
  componentWillMount() {
    LayoutAnimation.spring();
  }

  render() {
    var deviceWidth = Dimensions.get('window').width;
    const {onPress, img, pressed, backgroundColor, activeColor} = this.props;

    const styles = {
      image: {
        resizeMode: 'contain',
        width: /*pressed ? deviceWidth / 2 - 30 : deviceWidth / 2 - 20*/ deviceWidth / 2 - 20,
        height: /*pressed ? deviceWidth / 2 - 30 : deviceWidth / 2 - 20*/ deviceWidth / 2 - 20,
        borderRadius: 5,
        zIndex: 10
      },
      button: {
        marginBottom: 10,
        flexDirection: "column",
        alignItems: "center"
      },
      shadow: {
        height: pressed ? 15 : 20,
        width: /*pressed ? deviceWidth / 2 - 30 : deviceWidth / 2 - 20*/ deviceWidth / 2 - 20,
        marginTop: -5,
        backgroundColor: backgroundColor,
        borderBottomRightRadius: /*pressed ? 2 : 5*/ 5,
        borderBottomLeftRadius: /*pressed ? 2 : 5*/ 5,
        zIndex: 5,
      },
      outline: {
        width: deviceWidth / 2 - 10, 
        height: deviceWidth / 2 - 20,
        marginTop: -(deviceWidth / 2 - 25),
        marginLeft: 0,
        backgroundColor: pressed ? activeColor : "#333",
        zIndex: 1,
        borderRadius: 5
      },
      container: {
        width: deviceWidth / 2 - 20, 
        height: deviceWidth / 2, 
        justifyContent: "flex-end",
        alignItems: "center"
      }
    }
    return (
      <TouchableHighlight onPress={onPress} style={styles.button}>
        <View style={styles.container}>
          <Image style={styles.image} source={img}/>
          <View style={styles.shadow}/>
          <View style={styles.outline}/>
        </View>
      </TouchableHighlight>
    )
  }
}

AppRegistry.registerComponent('days', () => test);

function completeHabit(habit) {
  return fetch(`https://server-21days.herokuapp.com/device/track`, 
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
