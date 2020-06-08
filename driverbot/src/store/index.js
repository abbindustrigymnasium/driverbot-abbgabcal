import Vue from "vue"
import Vuex from "vuex"
Vue.use(Vuex)

const state = {
  broker: {
    name: "gabriel.calota@abbindustrigymnasium.se",
    password: "Kanin21",
    port: 8883,
    adress: "maqiatto.com"
  }
}

const mutations = {
  Set_broker(state, data) {
    state.broker = data
  },

  Update_: (state, broker) => {
    state.broker = broker
  }
}

const actions = {
  SetBroker: ({
    commit
  }, data) => {
    commit('Set_Broker', data)

  },

  Update: ({
    commit
  }, broker) => {
    commit('Update_', broker)
    console.log("Broker updated")
  }

}

const getters = {
  GetBroker: state => state.broker
}

export default new Vuex.Store({
  state,
  getters,
  mutations,
  actions,
});
