import React, { Component } from "react";
import { StatusBar } from "expo-status-bar";
import {
  StyleSheet,
  Text,
  View,
  ImageBackground,
  Dimensions,
  TouchableOpacity,
} from "react-native";

const { width, height } = Dimensions.get("window");

function caesarCipherNumber(input, shift) {
  let output = "";
  let numRegex = /^[0-9]+$/;

  for (let i = 0; i < input.length; i++) {
    let char = input[i];

    if (numRegex.test(char)) {
      let code = input.charCodeAt(i);
      let shiftedCode = ((code - 48 + shift) % 10) + 48;
      output += String.fromCharCode(shiftedCode);
    } else {
      output += char;
    }
  }

  return output;
}

function concatenateNumbers(numbers) {
  let concatenatedNumber = numbers.join("");
  return concatenatedNumber;
}

class App extends Component {
  constructor(props) {
    super(props);
    this.state = {
      passcode: [],
      size: 100,
      flag: 0,
      inputPassword: -1,
    };
    this.webSocketRef = React.createRef();
  }
  componentDidMount() {
    // Create WebSocket instance
    this.webSocketRef.current = new WebSocket("ws://192.168.18.30:8080");

    // Event listeners
    this.webSocketRef.current.onopen = () => {
      console.log("WebSocket connection opened");
    };

    this.webSocketRef.current.onclose = () => {
      console.log("WebSocket connection closed");
    };

    this.webSocketRef.current.onmessage = (event) => {
      console.log("Received message:", event.data);
    };

    this.webSocketRef.current.onerror = (error) => {
      console.error("WebSocket error:", error);
    };
  }

  changeSize = (size) => {
    let temp = 0;
    size = 0 ? (temp = 100) : (temp = 100 * (1 + size / 10));
    this.setState({ passcode: this.state.passcode, size: temp });
  };

  sendMessage = (message) => {
    console.warn("Sending password");
    if (
      this.webSocketRef.current &&
      this.webSocketRef.current.readyState === WebSocket.OPEN
    ) {
      this.webSocketRef.current.send(message);
    } else {
      console.error("WebSocket connection is not open.");
      return 0;
    }
    return 1;
  };

  onSend = () => {
    console.log(this.state.flag);
    if (this.state.passcode.length == 0) {
      console.warn("No input pressed");
      return;
    }

    console.log(this.state.passcode);
    let password = concatenateNumbers(this.state.passcode);

    if (
      (this.state.flag == 1) &
      (this.state.inputPassword != -1) &
      (password != this.state.inputPassword)
    ) {
      console.warn("Incorrect password");
      this.setState({ passcode: [] });
      return;
    }
    if (this.state.flag == 0) {
      let encryptedNumber = caesarCipherNumber(password, 6);
      let message = this.sendMessage(encryptedNumber);
      if (message == 1) {
        console.log(password);
        console.log(encryptedNumber);
        this.setState({ flag: 1, inputPassword: password, passcode: [] });
      } else {
        this.setState({ passcode: [] });
      }
      return;
    }
    if (
      (this.state.flag == 1) &
      (this.state.inputPassword != -1) &
      (password == this.state.inputPassword)
    ) {
      console.warn("Password correct!");
      this.setState({ passcode: [] });
      return;
    }
  };

  onPressed = (num) => {
    this.setState((prevState) => ({
      passcode: [...prevState.passcode, num],
    }));
  };
  render() {
    return (
      <View style={styles.container}>
        <StatusBar style="auto" />
        <ImageBackground
          source={require("./assets/background.png")}
          style={{
            position: "absolute",
            top: 0,
            width: width,
            height: height + 40,
          }}
          blurRadius={15}
        />
        {this.state.flag === 0 ? (
          <Text style={styles.mainText}>Input your password to secure it</Text>
        ) : null}
        <View>
          <View style={styles.numberContainer}>
            <View style={styles.row}>
              <TouchableOpacity onPress={() => this.onPressed(1)}>
                <View
                  style={{
                    ...styles.number,
                    height: this.state.size,
                    width: this.state.size,
                  }}
                >
                  <Text style={styles.text}>1</Text>
                </View>
              </TouchableOpacity>
              <TouchableOpacity onPress={() => this.onPressed(2)}>
                <View
                  style={{
                    ...styles.number,
                    height: this.state.size,
                    width: this.state.size,
                  }}
                >
                  <Text style={styles.text}>2</Text>
                </View>
              </TouchableOpacity>
              <TouchableOpacity onPress={() => this.onPressed(3)}>
                <View
                  style={{
                    ...styles.number,
                    height: this.state.size,
                    width: this.state.size,
                  }}
                >
                  <Text style={styles.text}>3</Text>
                </View>
              </TouchableOpacity>
            </View>
            <View style={styles.row}>
              <TouchableOpacity onPress={() => this.onPressed(4)}>
                <View
                  style={{
                    ...styles.number,
                    height: this.state.size,
                    width: this.state.size,
                  }}
                >
                  <Text style={styles.text}>4</Text>
                </View>
              </TouchableOpacity>
              <TouchableOpacity onPress={() => this.onPressed(5)}>
                <View
                  style={{
                    ...styles.number,
                    height: this.state.size,
                    width: this.state.size,
                  }}
                >
                  <Text style={styles.text}>5</Text>
                </View>
              </TouchableOpacity>
              <TouchableOpacity onPress={() => this.onPressed(6)}>
                <View
                  style={{
                    ...styles.number,
                    height: this.state.size,
                    width: this.state.size,
                  }}
                >
                  <Text style={styles.text}>6</Text>
                </View>
              </TouchableOpacity>
            </View>
            <View style={styles.row}>
              <TouchableOpacity onPress={() => this.onPressed(7)}>
                <View
                  style={{
                    ...styles.number,
                    height: this.state.size,
                    width: this.state.size,
                  }}
                >
                  <Text style={styles.text}>7</Text>
                </View>
              </TouchableOpacity>
              <TouchableOpacity onPress={() => this.onPressed(8)}>
                <View
                  style={{
                    ...styles.number,
                    height: this.state.size,
                    width: this.state.size,
                  }}
                >
                  <Text style={styles.text}>8</Text>
                </View>
              </TouchableOpacity>
              <TouchableOpacity onPress={() => this.onPressed(9)}>
                <View
                  style={{
                    ...styles.number,
                    height: this.state.size,
                    width: this.state.size,
                  }}
                >
                  <Text style={styles.text}>9</Text>
                </View>
              </TouchableOpacity>
            </View>
            <View style={styles.lastRow}>
              <TouchableOpacity onPress={() => this.onPressed(0)}>
                <View
                  style={{
                    ...styles.number,
                    height: this.state.size,
                    width: this.state.size,
                  }}
                >
                  <Text style={styles.text}>0</Text>
                </View>
              </TouchableOpacity>
              <TouchableOpacity onPress={() => this.onSend()}>
                <View
                  style={{
                    ...styles.number,
                    height: this.state.size,
                    width: this.state.size,
                  }}
                >
                  <Text style={styles.arrow}>→</Text>
                </View>
              </TouchableOpacity>
            </View>
          </View>
        </View>
        <View style={{ position: "absolute", bottom: 0, width: width }}>
          <View style={styles.row}>
            <TouchableOpacity onPress={() => this.changeSize(0)}>
              <View style={styles.size}>
                <Text style={styles.text}>T1</Text>
              </View>
            </TouchableOpacity>
            <TouchableOpacity onPress={() => this.changeSize(2)}>
              <View style={styles.size}>
                <Text style={styles.text}>T2</Text>
              </View>
            </TouchableOpacity>
            <TouchableOpacity onPress={() => this.changeSize(4)}>
              <View style={styles.size}>
                <Text style={styles.text}>T3</Text>
              </View>
            </TouchableOpacity>
          </View>
        </View>
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: "center",
    justifyContent: "center",
    height: height,
  },
  numberContainer: {
    width: width,
    marginTop: 10,
    display: "flex",
    flexDirection: "column",
  },
  row: {
    alignItems: "center",
    justifyContent: "center",
    display: "flex",
    flexDirection: "row",
    marginBottom: 20,
  },
  number: {
    width: 100,
    height: 100,
    borderRadius: 100,
    backgroundColor: "rgba(255,255,255,0.4)",
    display: "flex",
    alignContent: "center",
    justifyContent: "center",
    marginLeft: 10,
    marginRight: 10,
  },
  size: {
    width: 75,
    height: 75,
    borderRadius: 75,
    backgroundColor: "rgba(255,255,255,0.4)",
    display: "flex",
    alignContent: "center",
    justifyContent: "center",
    marginLeft: 10,
    marginRight: 10,
  },
  text: {
    textAlign: "center",
    color: "#000",
    fontSize: 30,
  },

  mainText: {
    textAlign: "center",
    color: "#f9f9f9",
    fontSize: 30,
    position: "absolute",
    top: 30,
  },

  arrow: {
    textAlign: "center",
    color: "#000",
    fontSize: 30,
    marginTop: -10,
  },
  lastRow: {
    display: "flex",
    flexDirection: "row",
    width: width,
    alignContent: "center",
    justifyContent: "center",
    textAlign: "center",
  },
});

export default App;
