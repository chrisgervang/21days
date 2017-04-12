import React, { Component } from 'react';
import { Day } from './Day'

import {
  StyleSheet,
  View, 
  Dimensions,
  ScrollView,
} from 'react-native';

export class Track extends Component {
    constructor(props) {
        super(props)
        this.state = {
            scroll: 0,
            days: 2
        }
    }
    scrollView;

    componentDidMount() {
        var x = Dimensions.get('window').width * (this.state.days - 1)
        this.scrollView.scrollTo({x: x, y: 0, animated: false})
        this.setState({scroll: x})
    }

    handleBack = () => {
        var x = this.state.scroll - Dimensions.get('window').width;
        if(x >= 0) {
        this.scrollView.scrollTo({x: x, y: 0, animated: true})
        this.setState({scroll: x})
        }
    }

    handleForward = () => {
        var x = this.state.scroll + Dimensions.get('window').width;
        if(x <= Dimensions.get('window').width * (this.state.days - 1)) {
        this.scrollView.scrollTo({x: x, y: 0, animated: true})
        this.setState({scroll: x})
        }
    }

    render() {
        return (
            <ScrollView ref={elt => this.scrollView = elt} indicatorStyle={"white"} contentContainerStyle={{flexDirection: "row"}} snapToAlignment={"center"} horizontal={true} pagingEnabled={true} >
                <Day key={1} date={"Yesterday"} onBack={this.handleBack} onForward={this.handleForward} canForward={true} canBack={false}/>
                <Day key={0} date={"Today"} onBack={this.handleBack} onForward={this.handleForward} canForward={false} canBack={true}/>
            </ScrollView>
        )
        
    }
}