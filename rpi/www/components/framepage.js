Vue.component('frame-page', function (resolve, reject) {
  axios.get("./views/" + globalConstants.framePage
                       + ".html?v=" + globalConstants.lastModifiedTimestamp).then(response => {
    resolve({
      template: response.data,
      props: {
        username: "",
        password: ""
      },
      data: function () {
        return {
          ViewTitle: ""
        }
      },
      methods: {
        created: function () {
        }
      },
      computed: {
      }
    })
  })
})
