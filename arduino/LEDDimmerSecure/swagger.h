/*
 * Research links:
 * https://techtutorialsx.com/2017/05/28/esp8266-adding-swagger-ui-to-rest-api/
 * 
 */

#ifndef Swagger_h
#define Swagger_h

const char * swaggerYAML = "{openapi: 3.0.1, info: {title: LED COB API, description: API for controlling COB LEDs, version: '0.1'}, servers: [{url"
                           ": /}], tags: [{name: leds, description: LED operations}, {name: pwr, description: Power operations}, {name: admin, des"
                           "cription: Admin operations}], paths: {/leds: {get: {tags: [leds], summary: Get all the LED values, responses: {'200': "
                           "{description: Success, content: {application/json: {examples: {various values: {value: [512, 1023, 0, 300, 0, 200]}}}}"
                           "}}, security: [{basicAuth: []}]}}, '/leds/{id}': {get: {tags: [leds], summary: Get the LED value, parameters: [{name: "
                           "id, in: path, required: true, schema: {type: integer}}], responses: {'200': {description: Success, content: {applicati"
                           "on/json: {examples: {min brightness: {value: {brightness: 0}}, max brightness: {value: {brightness: 1023}}}}}}}, secur"
                           "ity: [{basicAuth: []}]}}, '/leds/{id}/{value}': {put: {tags: [leds], summary: Set the LED value, parameters: [{name: i"
                           "d, in: path, required: true, schema: {type: integer}}, {name: value, in: path, required: true, schema: {type: integer}"
                           "}], responses: {'200': {description: Success, content: {}}}, security: [{basicAuth: []}]}}, /pwr: {get: {tags: [pwr], "
                           "summary: Get the power value, responses: {'200': {description: Success, content: {application/json: {examples: {power "
                           "off: {value: {power: 0}}, power on: {value: {power: 1}}}}}}}, security: [{basicAuth: []}]}}, '/pwr/{value}': {put: {ta"
                           "gs: [pwr], summary: Set the power on or off, parameters: [{name: value, in: path, required: true, schema: {type: integ"
                           "er}}], responses: {'200': {description: Success, content: {}}}, security: [{basicAuth: []}]}}, /forcelogin: {get: {tag"
                           "s: [admin], summary: Force Login, responses: {'401': {description: Success, content: {}}}}}}, components: {responses: "
                           "{UnauthorizedError: {description: Authentication information is missing or invalid, headers: {WWW_Authenticate: {schem"
                           "a: {type: string}}}, content: {}}}, securitySchemes: {basicAuth: {type: http, scheme: basic}}}}";

const char * swaggerUI = "<!DOCTYPE html><html lang=\"en\"><head> <meta charset=\"UTF-8\"> <meta name=\"viewport\" ""content=\"width=device-width,"
                         " initial-scale=1.0\"> <meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\"> <script src=\"//unpkg.com/swagger-ui-dis"
                         "t@3/swagger-ui-standalone-preset.js\"></script> <script src=\"//unpkg.com/swagger-ui-dist@3/swagger-ui-bundle.js\"></scr"
                         "ipt> <link rel=\"stylesheet\" href=\"//unpkg.com/swagger-ui-dist@3/swagger-ui.css\"/> <title>Swagger</title></head><body"
                         "> <div id=\"swagger-ui\"></div><script>window.onload=function(){SwaggerUIBundle({url: \"/swagger.yaml\",validatorUrl: nu"
                         "ll, dom_id: '#swagger-ui', presets: [ SwaggerUIBundle.presets.apis, SwaggerUIStandalonePreset], layout: \"StandaloneLayo"
                         "ut\"})}</script></body></html>";

#endif
