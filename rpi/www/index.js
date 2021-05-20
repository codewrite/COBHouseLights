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
      }
    },
    created: function() {
      var absLocation = window.location.href;
      globalConstants.framePage = 	absLocation.replace(/^.*page=(\w+).*$/, '$1');
  
      if (globalConstants.framePage === absLocation || globalConstants.framePage === 'home') {
        globalConstants.framePage = 'ledcontrol';
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
