Vue.component('led-control', {
    props: {
      username: "",
      password: ""
    },
    data: function () {
      return {
        //ledControlUrl: "http://wemos1/leds",
        ledControlUrl: "http://192.168.1.211/leds",
        ledBrightness: []
      }
    },
    template: /*html*/`
    <div class="strippedLedList">
      <div v-for="(n, i) in 6" class="row">
        <div class="col-sm-1">
          <label :for="'ledSlider' + n">LED {{ n }}</label>
        </div>
        <div class="col-sm-4">
          <input type="range" class="custom-range" :id="'ledSlider' + n" min="0" max="1023" v-model="ledBrightness[i]" @change="ledBrightnessChanged(n, ledBrightness[i])">
        </div>
        <div class="col-sm-1 ledStriped">
          {{ ledBrightness[i] }}
        </div>
      </div>
    </div>
    `,
    mounted() {
    },
    watch: {
    },
    created: function() {
      this.getLeds();
      for (i=0; i<6; i++)
      {
        this.ledBrightness[i] = 0;
      }
    },
    methods: {
      setLedBrightness: function (led, brightness) {
        axios
          .put(this.ledControlUrl + "/" + led, { brightness: brightness }, { auth: { username: this.username, password: this.password } })
          .then(response => {
          })
          .catch(error => {
            console.log(error)
          })
          .finally(() => { });
      },
      getLeds: function() {
        axios
          .get(this.ledControlUrl, { auth: { username: this.username, password: this.password } })
          .then(response => {
            for (i=0; i<6; i++)
            {
              Vue.set(this.ledBrightness, i, response.data[i].brightness);
            }
          })
          .catch(error => {
            console.log(error)
          })
          .finally(() => { });
      },
      ledBrightnessChanged(led, brightness) {
        this.setLedBrightness(led, brightness);
      }
    },
    computed: {
    }
})
