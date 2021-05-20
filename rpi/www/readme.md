# Notes on the VueJS app

- I am loading scripts dynamically so that I can version the includes. This allows me to have a single version number. Also, whenever the index.html changes
(which it dies when the version number changes) all files get a different url. This prevents any browsers from caching old js files.
