//api/index.js
var socket = new WebSocket("ws://localhost:3000/ws");

//interacts with out server that was setup in main.go

//this function connects to the websocket endpoint
//and listens for events.
let connect = cb =>{
    console.log("Attempting Connection...")

    socket.onopen = () => {
        console.log("Successfully Connected");
    };

    socket.onmessage = msg => {
        console.log(msg);
        cb(msg);
    };

    socket.onclose = event =>{
        console.log("Socket Closed Connection: ", event);
    };

    socket.onerror = error => {
        console.log("Socket Error: ", error);
    };
};

//this function allows us to send messages from our frontend
//to our backend via our WebSocket using socket.send(msg)

let sendMsg = msg => {
    console.log("sending msg: ", msg);
    socket.send(msg);
};

export { connect, sendMsg };

//need to update the app.js to add these functions