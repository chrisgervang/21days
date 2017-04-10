import React, { Component } from 'react';
import { api_key, device_id } from './token';
import {Button} from './src/Button'

import {
  AppRegistry,
  StyleSheet,
  View, 
  Dimensions,
  StatusBar,
  ScrollView,
  Text,
  DatePickerIOS,
  LayoutAnimation
} from 'react-native';

function parseText(response) {
  return response.text()
}

export default class test extends Component {
  scrollView;
  componentDidMount() {
    console.log("Scroll!", !!this.scrollView.scrollTo, "test")
    this.scrollView.scrollTo({x: 0, y: 0, animated: false})
  }

  render() {
    return (
      <View style={{flex: 1, backgroundColor: 'black'}}>
        <StatusBar
          barStyle='light-content'
        />
        <ScrollView ref={elt => { console.log("View!", !!elt); this.scrollView = elt;}} indicatorStyle={"white"} contentContainerStyle={{flexDirection: "row"}} snapToAlignment={"center"} horizontal={true} pagingEnabled={true} >
          <Day key={2} date={"Thursday"}/>
          <Day key={1} date={"Yesterday"}/>
          <Day key={0} date={"Today"}/>
        </ScrollView>
        
      </View>
    )
  }
}

class Day extends Component {
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
      <View style={{paddingLeft: 10, paddingRight: 10}}>
        <View style={{flex: 1, flexDirection: 'row'}}>
          <View style={{flex: 1, flexDirection: 'column', justifyContent: 'center', alignItems: 'center'}}>
              <Button onPress={this.handlePressButtonBrush} img={require('./img/brush.jpg')} pressed={this.state.brush}
                backgroundColor="#8A7EA8" activeColor="rgb(144, 0, 255)"/>
              <Button onPress={this.handlePressButtonTime} img={require('./img/onTime.jpg')} pressed={this.state.onTime}
                backgroundColor="#E5CBA8" /*FFDEB3*/ activeColor="rgb(255, 255, 0)"/>
              <Button onPress={this.handlePressButtonMurder} img={require('./img/murder.jpg')} pressed={this.state.dontMurder}
                backgroundColor="#0E1019" activeColor="rgb(255, 0, 25)"/>
          </View>
          <View style={{paddingLeft: 20, flex: 1, flexDirection: 'column', justifyContent: 'center', alignItems: 'center'}}>
              <Button onPress={this.handlePressButtonSleep} img={require('./img/sleep.jpg')} pressed={this.state.sleep}
                backgroundColor=/*"#17344C"*/ "#081119" activeColor="rgb(0, 158, 255)"/>
              <Button onPress={this.handlePressButtonWorkout} img={require('./img/workout.jpg')} pressed={this.state.workout}
                backgroundColor="#325866" activeColor="rgb(255, 127, 0)"/>
              <Button onPress={this.handlePressButtonSweets} img={require('./img/sweets.jpg')} pressed={this.state.noSweets}
                backgroundColor="#74A574" activeColor="rgb(0, 255, 0)"/>
          </View>
        </View>
        <View style={{alignItems: "center"}}>
          <DatePickerIOS date={new Date()} onDateChange={console.log}/>
          <Text style={{color: "white"}}>
            {this.props.date}
          </Text>
        </View>
      </View>
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
