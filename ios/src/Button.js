import React, { Component } from 'react';
import {
  StyleSheet,
  Text,
  TouchableHighlight,
  View, 
  Image,
  Dimensions,
  LayoutAnimation
} from 'react-native';
export class Button extends Component {
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