// Script for initial loading of Vue components
function includeJS(uri) {
    var def = $.Deferred();
    var script = document.createElement('script');
    script.src = uri;
    script.onload = function() {
        def.resolve(uri);
    }
    script.onerror = function() {
        def.reject();
    }
    document.body.appendChild(script);
    return def.promise();
}
