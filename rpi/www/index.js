//--------------------------------
// Main App
//--------------------------------

var app = new Vue({
    el: '#app',
    props: {
    },
    data: {
      username: "",
      password: "",
      loggedIn: false
    },
    methods: {
      pageVersion: function() {
        if (globalConstants === undefined) { return "" }
          return globalConstants.indexPageVersion;
      },
      lastModified: function() {
        return new Date(document.lastModified).toDateString()
      },
      login: function() {
        this.loggedIn = true;
      },
      logout: function() {
        this.loggedIn = false;
        this.username = "";
        this.password = "";
      }
    },
    mounted() {
      if (localStorage.username) {
        this.username = JSON.parse(localStorage.username);
      }
      if (localStorage.password) {
        this.password = JSON.parse(localStorage.password);
      }
      if (localStorage.loggedIn) {
        this.loggedIn = (localStorage.loggedIn == String(true));
      }
    },
    watch: {
      username: {
        handler(newValue) {
          localStorage.username = JSON.stringify(newValue);
        },
        deep: true
      },
      password: {
        handler(newValue) {
          localStorage.password = JSON.stringify(newValue);
        },
        deep: true
      },
      loggedIn(newValue) {
        localStorage.loggedIn = newValue;
      },
    },
    created: function() {
      var absLocation = window.location.href;
      globalConstants.framePage = 	absLocation.replace(/^.*page=(\w+).*$/, '$1');
  
      if (globalConstants.framePage === absLocation || globalConstants.framePage === 'home') {
        globalConstants.framePage = 'onoffcontrol';
      }
      $('#ex1').slider({
        formatter: function(value) {
          return 'Current value: ' + value;
        }
      });
    },
    computed: {
    }
})
