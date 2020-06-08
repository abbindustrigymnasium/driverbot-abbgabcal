<template>
    <v-container>
        <v-btn v-if="connected == false" color="red" @click="connect()">
            Connect
        </v-btn>
        <v-btn v-else color="green" disabled>
            Connected till broker
        </v-btn>
        <v-card class="flexcard" id=ButtonCard flat>
            <v-card class="white--text" flat>
                <v-card-title v-if="connected==true">Connected to broker</v-card-title>
                <v-card-title v-else>Not connected to broker</v-card-title>
                <v-list class="LoggerStyle">
                    <div v-for="(item) in log" :key="item">{{"> "+ item}}</div>
                </v-list>
            </v-card>
            <v-layout row wrap justify-center id=LayoutStyle>
                <v-flex xs12 md12>
                    <v-btn class="fontcolour--text" color="secondary" id=DirectionStyle depressed
                        @mousedown="Send('motor', 'fram')">
                        <v-icon>mdi-chevron-up</v-icon>
                    </v-btn>
                </v-flex>
                <v-flex xs6 md6>
                    <v-btn class="fontcolour--text" color="secondary" id="TurnStyle" depressed
                        @click="Send('motor', 'left')">
                        <v-icon>mdi-chevron-left</v-icon>
                    </v-btn>
                </v-flex>
                <v-flex xs6 md6>
                    <v-btn class="fontcolour--text" color="secondary" id="TurnStyle" depressed
                        @click="Send('motor', 'right')">
                        <v-icon>mdi-chevron-right</v-icon>
                    </v-btn>
                </v-flex>
                <v-flex xs12 md12>
                    <v-btn class="fontcolour--text" color="secondary" id=DirectionStyle depressed
                        @click="Send('motor', 'bak')">
                        <v-icon>mdi-chevron-down</v-icon>
                    </v-btn>
                </v-flex>
            </v-layout>
        </v-card>
        <v-btn v-if="Keyboard != true" id=KeyboardBtnStyle color=red block @click="KeyboardToggle()">Keyboard controll
            off</v-btn>
        <v-btn v-else id=KeyboardBtnStyle color=green block @click="KeyboardToggle()">Keyboard controll on</v-btn>




    </v-container>
</template>

<script>
import {
    connect
} from 'mqtt'
var mqtt = require("mqtt")
export default {
    name: "Controller",

    data() {
        return {
            connected: false,
            clientId: "Nothing",
            client: null,
            options: {},
            log: [" "],
            Keyboard: null
        }
    },

    mounted() {

    },

    computed: {

    },

    methods: {
        connect() {  //Metod som ansluter till MQTT, aktiveras med "Connect"-knappen
            var ref = this
            let Broker = this.$store.getters.GetBroker
            this.clientId = "WebController" + Math.random().toString(16).substr(2, 8) //kontrollerns ID
            var url = "mqtt://" + Broker.adress
            var options = {
                port: Broker.port,
                clientId: this.clientId,
                username: Broker.name,
                password: Broker.password
            }
            this.options = options

            this.client = mqtt.connect(url, options) //Anropar connect-funktionen i MQQT.js biblioteket



            this.client
                .on("connect", function () {
                    ref.Connecting(true)
                })
                .on("error", function () {
                    ref.Connecting(false)
                })
                .on("close", function () {
                    ref.Connecting(false)
                })


            this.client.on('message', function (topic, message) {
                ref.AddToLogger(message)
            })


        },

        Connecting(connected) {
            this.connected = connected

            if (connected == false) {
                console.log("Not connected")
            } else {
                this.Send("motor", this.clientId + " har anslutits. ")
                this.client.subscribe(this.options.username + '/motor/logger', {  //Prenumererar på logger-topic
                    qos: 0,
                    nl: true
                }, function (err) {
                    if (!err) {
                        console.log("Har prenumererat")
                    }
                })

            }
        },

        Send(adress, message) {  //Funktion som skickar till MQTT
            this.client.publish(this.options.username + "/" + adress, message)
            console.log("Sending " + message + " to mqtt")
        },

        AddToLogger(message) {  //Funktion som körs när ett meddelande tas emot på logger, lägger till tiden samt vad bilen gjort i logger
            var today = new Date()
            var time = today.getHours() + ":" + today.getMinutes() + ":" + today.getSeconds() + ":" + today.getMilliseconds();
            var splicer = this.log.length - 1
            this.log.splice(splicer, 0, time + ":  " + message)
            var container = document.querySelector(".LoggerStyle") //Denna rad samt nedanstående 2 rader gör så att loggfönstret automatiskt scrollar nedåt med hjälp av CSS
            var scrollHeight = container.scrollHeight
            container.scrollTop = scrollHeight


        },

        KeyboardToggle() { //Funktion som sätter på/av tangentbordskontroll
            if (this.Keyboard == null) {
                this.Keyboard = true
                window.addEventListener("keypress", e => this.KeyboardControl(e)) //Sätter på tangentbordskontroll och kör funktionen Keyboardkontroll när en tangent trycks
            } else if (this.Keyboard == false) { //Stänger av tangentbordskontroll
                this.Keyboard = true

            } else {
                this.Keyboard = false
            }
        },

        KeyboardControl(e) { //Kollar vilken tangent som har tryckts ned och skickar den till bilen med MQTT
            if (this.Keyboard == true) {
                let keydown = String.fromCharCode(e.keyCode)
                if (this.connected == false) {
                    alert("Not connected to MQTT")
                } else if (keydown == 'w') {
                    this.Send('motor', 'fram')
                } else if (keydown == "s") {
                    this.Send('motor', 'bak')
                } else if (keydown == "a") {
                    this.Send('motor', 'left')
                } else if (keydown == "d") {
                    this.Send('motor', 'right')
                }
            }
        }


    }
}

</script>

<style scoped>
#ButtonCard {
    height: 100%;
    width: 400px;
    background-color: transparent;

}

#DirectionStyle {
    margin-left: 40%;
    width: 20%;
}

#TurnStyle {
    width: 20%;
    margin-left: 30%;
}

.LoggerStyle {
    height: 192px;
    max-width: 400px;
    overflow-y: scroll;
}

#LayoutStyle {
    margin-top: 24px;
}

#KeyboardBtnStyle {
    margin-top: 24px;
}
</style>
