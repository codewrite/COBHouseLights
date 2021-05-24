Vue.component('on-off-control', {
    props: {
      username: "",
      password: ""
    },
    data: function () {
      return {
        //ledControlUrl: "https://wemos1",
        ledControlUrl: "https://192.168.1.211",
        ledOn: []
      }
    },
    template: /*html*/`
    <div>
      <div v-for="(n, i) in 6" style="float:left">
        <button type="button" :class="ledOn[i] ? 'btn-primary' : 'btn-outline-primary' + ' btn'" style="width:250px;height:150px;margin:10px;font-size:50px" @click="toggleLed(n,i)">LED{{n}}</button>
      </div>
    </div>
    `,
    mounted() {
    },
    watch: {
    },
    created: function() {
      this.getLeds();
      this.setPwr(true);
    },
    methods: {
      getbuttonClasses(led) {
        var clsStr = "btn ";
        clsStr += this.ledOn[led] ? 'btn-primary' : 'btn-outline-primary';
        return clsStr;
      },
      ledIsOn(led) {
        return this.ledOn[led];
      },
      getLeds: function() {
        axios
          .get(this.ledControlUrl + "/leds", { auth: { username: this.username, password: this.password } })
          .then(response => {
            for (i=0; i<6; i++)
            {
              Vue.set(this.ledOn, i, response.data[i].on);
            }
          })
          .catch(error => {
            console.log(error)
          })
          .finally(() => { });
      },
      toggleLed: function(num, led) {
        Vue.set(this.ledOn, led, !this.ledOn[led]);
        axios
          .put(this.ledControlUrl + "/leds/" + num, { on: this.ledOn[led] }, { auth: { username: this.username, password: this.password } })
          .then(response => {
          })
          .catch(error => {
            console.log(error)
          })
          .finally(() => { });
      },
      setPwr: function(on) {
        axios
          .put(this.ledControlUrl + "/pwr/" + (on ? "1" : "0"), {}, { auth: { username: this.username, password: this.password } })
          .then(response => {
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
