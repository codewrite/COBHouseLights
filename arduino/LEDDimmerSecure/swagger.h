/*
 * Research links:
 * https://github.com/swagger-api/swagger-ui
 * https://unpkg.com/browse/swagger-ui/
 * https://unpkg.com/browse/swagger-ui-dist/
 * https://stackoverflow.com/questions/55733609/display-swagger-ui-on-flask-without-any-hookups
 * https://techtutorialsx.com/2017/05/28/esp8266-adding-swagger-ui-to-rest-api/  (above links show an easy way than this - this link included for reference only)
 * SwaggerUIBundle validatorUrl: null => to get rid of error if yaml file is not publicly accessible
 * 
 */

#ifndef Swagger_h
#define Swagger_h

const char * swaggerYAML = "{openapi: 3.0.1, info: {title: LED COB API, description: API for controlling COB LEDs, version: '0.1'}, servers: [{url:"
                           " /}], tags: [{name: leds, description: LED operations}, {name: pwr, description: Power operations}, {name: admin, descri"
                           "ption: Admin operations}], paths: {/leds: {get: {tags: [leds], summary: Get all the LED values, responses: {'200': {desc"
                           "ription: Success, content: {application/json: {examples: {various values: {value: [{'on': true, brightness: 512}, {'on':"
                           " true, brightness: 1023}, {'on': false, brightness: 0}, {'on': false, brightness: 300}, {'on': true, brightness: 0}, {'o"
                           "n': false, brightness: 200}]}}}}}}, security: [{basicAuth: []}]}}, '/leds/{id}': {get: {tags: [leds], summary: Get the L"
                           "ED value, parameters: [{name: id, in: path, required: true, schema: {type: integer}}], responses: {'200': {description: "
                           "Success, content: {application/json: {examples: {'min brightness, LED on': {value: {'on': true, brightness: 0}}, 'max br"
                           "ightness, LED on': {value: {'on': true, brightness: 1023}}, 'brightness set, LED off': {value: {'on': false, brightness:"
                           " 500}}}}}}}, security: [{basicAuth: []}]}, put: {tags: [leds], summary: Set the LED value, parameters: [{name: id, in: p"
                           "ath, required: true, schema: {type: integer}}], requestBody: {description: Update LED settings, required: true, content:"
                           " {application/json: {schema: {$ref: '#/components/schemas/Led'}, examples: {'LED on, mid level brightness': {value: {'on"
                           "': true, brightness: 512}}, LED off: {value: {'on': false}}, 'LED on, use last brightness': {value: {'on': true}}}}}}, r"
                           "esponses: {'200': {description: Success, content: {}}}, security: [{basicAuth: []}]}}, /pwr: {get: {tags: [pwr], summary"
                           ": Get the power value, responses: {'200': {description: Success, content: {application/json: {examples: {power off: {val"
                           "ue: {power: 0}}, power on: {value: {power: 1}}}}}}}, security: [{basicAuth: []}]}}, '/pwr/{value}': {put: {tags: [pwr], "
                           "summary: Set the power on or off, parameters: [{name: value, in: path, required: true, schema: {type: integer}}], respon"
                           "ses: {'200': {description: Success, content: {}}}, security: [{basicAuth: []}]}}, /forcelogin: {get: {tags: [admin], sum"
                           "mary: Force Login, responses: {'401': {description: Success, content: {}}}}}}, components: {responses: {UnauthorizedErro"
                           "r: {description: Authentication information is missing or invalid, headers: {WWW_Authenticate: {schema: {type: string}}}"
                           ", content: {}}}, securitySchemes: {basicAuth: {type: http, scheme: basic}}, schemas: {Led: {type: object, properties: {'"
                           "on': {type: boolean}, brightness: {type: integer, format: int32}}}}}}";

const char * swaggerUI = "<!DOCTYPE html><html lang=\"en\"><head> <meta charset=\"UTF-8\"> <meta name=\"viewport\" ""content=\"width=device-width,"
                         " initial-scale=1.0\"> <meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\"> <script src=\"//unpkg.com/swagger-ui-dis"
                         "t@3/swagger-ui-standalone-preset.js\"></script> <script src=\"//unpkg.com/swagger-ui-dist@3/swagger-ui-bundle.js\"></scr"
                         "ipt> <link rel=\"stylesheet\" href=\"//unpkg.com/swagger-ui-dist@3/swagger-ui.css\"/> <title>Swagger</title></head><body"
                         "> <div id=\"swagger-ui\"></div><script>window.onload=function(){SwaggerUIBundle({url: \"/swagger.yaml\",validatorUrl: nu"
                         "ll, dom_id: '#swagger-ui', presets: [ SwaggerUIBundle.presets.apis, SwaggerUIStandalonePreset], layout: \"StandaloneLayo"
                         "ut\"})}</script></body></html>";

#endif
