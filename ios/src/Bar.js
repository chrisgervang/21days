import React, { Component } from 'react';
import Icon from 'react-native-vector-icons/Entypo';
import {
  StyleSheet,
  View, 
  Text,
  TouchableOpacity
} from 'react-native';

export const Bar = ({ label, onForward, onBack, canBack, canForward }) => (
  <View style={{ flexDirection: "row", alignItems: "center", marginBottom: 10 }}>
    <View style={{alignItems: "flex-start", justifyContent: "center", width: 24 }}>
      { canBack &&
        <TouchableOpacity onPress={onBack}>
          <Icon name="chevron-thin-left" size={24} color="#FFF"/>
        </TouchableOpacity>
      }
    </View>
    <View style={{alignItems: "center", justifyContent: "center", flex: 1}}>
      <Text style={{color: "white", fontSize: 24, flex: 3}}>
        {label}
      </Text>
    </View>
    <View style={{alignItems: "flex-end", justifyContent: "center", width: 24}}>
      { canForward &&
        <TouchableOpacity onPress={onForward}>
          <Icon name="chevron-thin-right" size={24} color="#FFF"/>
        </TouchableOpacity>
      }
    </View>
  </View>
)