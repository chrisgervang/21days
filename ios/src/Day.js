import React, { Component } from 'react';
import { Button } from './Button'
import { Bar } from './Bar'
import { completeHabit, getHistory } from './api'
import {
  StyleSheet,
  View, 
} from 'react-native';

export class Day extends Component {
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
    completeHabit('brush twice').then(text => {
        if(text === "Already Done For Today") {        
            LayoutAnimation.spring();
            this.setState({brush: true});
        }
    })
    LayoutAnimation.spring();
    this.setState({brush: true});
  }

  handlePressButtonTime = () => {
    console.log("TIME")
    completeHabit('on time').then(text => {
        if(text === "Already Done For Today") {        
            LayoutAnimation.spring();
            this.setState({onTime: true});
        }
    })
    LayoutAnimation.spring();
    this.setState({onTime: true});
  }

  handlePressButtonMurder = () => {
    console.log("MURDER")
    completeHabit('dont murder').then(text => {
        if(text === "Already Done For Today") {
          LayoutAnimation.spring();
          this.setState({dontMurder: true});
        }
    })
    LayoutAnimation.spring();
    this.setState({dontMurder: true});
  }

  handlePressButtonSleep = () => {
    console.log("SLEEP")
    completeHabit('sleep by 12am').then(text => {
        if(text === "Already Done For Today") {
          this.setState({sleep: true});
        }
    })
    LayoutAnimation.spring();
    this.setState({sleep: true});
  }

  handlePressButtonWorkout = () => {
    console.log("WORKOUT")
    completeHabit('workout').then(text => {
        if(text === "Already Done For Today") {
          LayoutAnimation.spring();
          this.setState({workout: true});
        }
    })
    LayoutAnimation.spring();
    this.setState({workout: true});
  }

  handlePressButtonSweets = () => {
    console.log("SWEETS")
    completeHabit('no sweets').then(text => {
        console.log(text)
        if(text === "Already Done For Today") {
          LayoutAnimation.spring();
          this.setState({noSweets: true});
        }
    })
    LayoutAnimation.spring();
    this.setState({noSweets: true});
  }

  componentDidMount() {
    getHistory().then(json => {
        console.log(json)
    })
  }

  render() {
    const { onBack, onForward, canBack, canForward, date } = this.props
    return (
      <View style={{paddingLeft: 10, paddingRight: 10, justifyContent: "space-between"}}>
        <View style={{flexDirection: 'row'}}>
          <View style={{flex: 1, flexDirection: 'column', justifyContent: 'center', alignItems: 'center'}}>
              <Button onPress={this.handlePressButtonBrush} img={require('../img/brush.jpg')} pressed={this.state.brush}
                backgroundColor="#8A7EA8" activeColor="rgb(144, 0, 255)"/>
              <Button onPress={this.handlePressButtonTime} img={require('../img/onTime.jpg')} pressed={this.state.onTime}
                backgroundColor="#E5CBA8" /*FFDEB3*/ activeColor="rgb(255, 255, 0)"/>
              <Button onPress={this.handlePressButtonMurder} img={require('../img/murder.jpg')} pressed={this.state.dontMurder}
                backgroundColor="#0E1019" activeColor="rgb(255, 0, 25)"/>
          </View>
          <View style={{paddingLeft: 20, flex: 1, flexDirection: 'column', justifyContent: 'center', alignItems: 'center'}}>
              <Button onPress={this.handlePressButtonSleep} img={require('../img/sleep.jpg')} pressed={this.state.sleep}
                backgroundColor=/*"#17344C"*/ "#081119" activeColor="rgb(0, 158, 255)"/>
              <Button onPress={this.handlePressButtonWorkout} img={require('../img/workout.jpg')} pressed={this.state.workout}
                backgroundColor="#325866" activeColor="rgb(255, 127, 0)"/>
              <Button onPress={this.handlePressButtonSweets} img={require('../img/sweets.jpg')} pressed={this.state.noSweets}
                backgroundColor="#74A574" activeColor="rgb(0, 255, 0)"/>
          </View>
        </View>
        <Bar label={ date } onForward={onForward} onBack={onBack} canForward={canForward} canBack={canBack} />
      </View>
    )
  }
}

