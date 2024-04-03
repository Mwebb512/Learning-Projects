import React, {Component } from "react";
import "./ChatHistory";

class ChatHistory extends Component {
    render(){
        //iterates over each element in the chatHistory map and stores 
        //the messages in the messages array
        const messages = this.props.chatHistory.map((msg, index) => (
            <p key={index}>{msg.data}</p>
        ));

        return (
            <div className= "ChatHistory">
                <h2>Chat History</h2>
                {messages}
            </div>
        );
    }
}

export default ChatHistory;