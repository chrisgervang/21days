import React, { Component } from 'react';
import { Button } from './Button'
import { Bar } from './Bar'
import { trackHabit } from './api'
import { selectors as historySelectors } from '../store/history'
import { connect } from 'react-redux'
import {
  StyleSheet,
  View,
  LayoutAnimation
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
    trackHabit('brush twice', this.props.daysAgo, !this.state.brush).then(text => {
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
    trackHabit('on time', this.props.daysAgo, !this.state.onTime).then(text => {
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
    trackHabit('dont murder', this.props.daysAgo, !this.state.dontMurder).then(text => {
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
    trackHabit('sleep by 12am', this.props.daysAgo, !this.state.sleep).then(text => {
        if(text === "Already Done For Today") {
          this.setState({sleep: true});
        }
    })
    LayoutAnimation.spring();
    this.setState({sleep: true});
  }

  handlePressButtonWorkout = () => {
    console.log("WORKOUT")
    trackHabit('workout', this.props.daysAgo, !this.state.workout).then(text => {
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
    trackHabit('no sweets', this.props.daysAgo, !this.state.noSweets).then(text => {
        console.log(text)
        if(text === "Already Done For Today") {
          LayoutAnimation.spring();
          this.setState({noSweets: true});
        }
    })
    LayoutAnimation.spring();
    this.setState({noSweets: true});
  }

  componentDidUpdate(nextProps) {
      if(this.props.status !== nextProps.status) {
        this.setState({
            brush: this.props.status["brush twice"],
            onTime: this.props.status["on time"],
            sleep: this.props.status["sleep by 12am"],
            workout: this.props.status["workout"],
            noSweets: this.props.status["no sweets"],
            dontMurder: this.props.status["dont murder"]
        })
      }
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

export default connect((state, ownProps) => ({
    status: historySelectors.fromDaysAgo(state, ownProps.daysAgo)
}), {})(Day)

