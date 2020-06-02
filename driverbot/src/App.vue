<template>
  <v-app>
    <v-app-bar app>
      <v-toolbar-title>Drivercontroller</v-toolbar-title>
      <v-spacer></v-spacer>
      <v-btn class="fontcolour--text" color="secondary" depressed @click.stop="dialog = true">
        <span>Connection settings</span>
        <v-icon id=connectionstyle>mdi-access-point</v-icon>
      </v-btn>

    </v-app-bar>
    <v-content>
      <v-dialog v-model="dialog" max-width="290" height="290">
        <v-card elevation-12>
          <v-card-title class="headline">Connection to MQTT</v-card-title>
          <v-text-field v-model="name" label="Broker Name" :placeholder="name"></v-text-field>
          <v-text-field v-model="password" label="Broker Password"></v-text-field>
          <v-text-field v-model="port" label="Broker Port"></v-text-field>
          <v-text-field v-model="adress" label="Broker Adress"></v-text-field>

          <v-card-actions>
            <v-btn class="fontcolour--text" color="secondary" block @click="update()">Update Broker settings</v-btn>
          </v-card-actions>



        </v-card>
      </v-dialog>

      <Controller />
    </v-content>

  </v-app>
</template>

<script>
import Controller from "./components/Controller";

export default {
  name: "App",

  components: {
    Controller
  },

  data() {
    return {
      dialog: false,
      name: "",
      password: "",
      port: "",
      adress: ""
    }
  },

  created() {
    let broker = this.$store.getters.GetBroker
    this.name = broker.name
    this.password = broker.password
    this.port = broker.port
    this.adress = broker.adress

  },

  methods: {
    update() {
      let Broker = {
        name: this.name,
        password: this.password,
        port: this.port,
        adress: this.adress
      }
      console.log(Broker)
      this.$store.dispatch("Update")
      this.dialog = false
    }

  }
};
</script>

<style scoped>
#connectionstyle {
  padding-left: 10px;
}
</style>