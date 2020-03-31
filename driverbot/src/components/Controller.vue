<template>
    <v-container justify-center align-center>
            <v-card id="ButtonCard" color="blue" align-center>
            <v-layout row wrap justify-center>
                <v-flex xs12 md12>
                    <v-btn id=DirectionStyle>
                        <v-icon>mdi-chevron-up</v-icon>
                    </v-btn>
                </v-flex>
                <v-flex xs6 md6>
                    <v-btn id="TurnStyle">
                        <v-icon>mdi-chevron-left</v-icon>
                    </v-btn>
                </v-flex>
                <v-flex xs6 md6>
                    <v-btn id="TurnStyle">
                        <v-icon>mdi-chevron-right</v-icon>
                    </v-btn>
                </v-flex>
                <v-flex xs12 md12>
                    <v-btn id=DirectionStyle>
                        <v-icon>mdi-chevron-down</v-icon>
                    </v-btn>
                </v-flex>
            </v-layout>
        </v-card>
        
        
    </v-container>
</template>

<script>
var mqtt = require("mqtt")
export default {
    name: "Controller",

    data() {
        return {
        }
    }
}

</script>

<style scoped>
/* #directionbutton { */
    /* size="20px"
} */
#ButtonCard {
    height: 100%;
    width: 100%;
}
#DirectionStyle {
    margin-left: 40%;
    width: 20%;
    height: 100%;
}
#TurnStyle {
    width: 20%;
    margin-left: 30%;
}
</style>
