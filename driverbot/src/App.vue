<template>
  <v-app>
    <v-app-bar app color="primary" dark>

      <v-spacer></v-spacer>
        <span class="mr-2">Latest Release</span>
        <v-icon>mdi-open-in-new</v-icon> 
    </v-app-bar>
        <v-content>
      <Controller/>
    </v-content>

  </v-app>
</template>

<script>
// import HelloWorld from "./components/HelloWorld";
import Controller from "./components/Controller";

export default {
  name: "App",

  components: {
    Controller
  },

  data: () => ({
    //
  })
};
</script>
