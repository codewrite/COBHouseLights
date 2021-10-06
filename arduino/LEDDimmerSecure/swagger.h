/*
 * Research links:
 * https://github.com/swagger-api/swagger-ui
 * https://unpkg.com/browse/swagger-ui/
 * https://unpkg.com/browse/swagger-ui-dist/
 * https://stackoverflow.com/questions/55733609/display-swagger-ui-on-flask-without-any-hookups
 * https://techtutorialsx.com/2017/05/28/esp8266-adding-swagger-ui-to-rest-api/  (above links show an easy way than this - this link included for reference only)
 * SwaggerUIBundle validatorUrl: null => to get rid of error if yaml file is not publicly accessible
 */

#ifndef Swagger_h
#define Swagger_h

// ****DO NOT EDIT DIRECTLY****
// Instead, get the file api-yaml.txt from swagger-src folder and follow the instructions in that file.
const char * swaggerYAML = "{openapi: 3.0.1, info: {title: LED COB API, description: API for controlling COB LEDs, version: '0.1'}, servers: [{url:"
                           " /}], tags: [{name: leds, description: LED operations}, {name: pwr, description: Power operations}, {name: status, descr"
                           "iption: Status operations}, {name: admin, description: Admin operations}], paths: {/leds: {get: {tags: [leds], summary: "
                           "Get all the LED values, responses: {'200': {description: Success, content: {application/json: {schema: {type: array, ite"
                           "ms: {$ref: '#/components/schemas/Led'}}, examples: {various values: {value: [{'on': true, brightness: 512}, {'on': true,"
                           " brightness: 1023}, {'on': false, brightness: 0}, {'on': false, brightness: 300}, {'on': true, brightness: 0}, {'on': fa"
                           "lse, brightness: 200}]}}}}}}, security: [{basicAuth: []}]}}, '/leds/{id}': {get: {tags: [leds], summary: Get the LED val"
                           "ue, parameters: [{name: id, in: path, required: true, schema: {type: integer}}], responses: {'200': {description: Succes"
                           "s, content: {application/json: {schema: {$ref: '#/components/schemas/Led'}, examples: {'min brightness, LED on': {value:"
                           " {'on': true, brightness: 0}}, 'max brightness, LED on': {value: {'on': true, brightness: 1023}}, 'brightness set, LED o"
                           "ff': {value: {'on': false, brightness: 500}}}}}}}, security: [{basicAuth: []}]}, put: {tags: [leds], summary: Set the LE"
                           "D value, parameters: [{name: id, in: path, required: true, schema: {type: integer}}], requestBody: {description: Update "
                           "LED settings, required: true, content: {application/json: {schema: {$ref: '#/components/schemas/Led'}, examples: {'LED o"
                           "n, mid level brightness': {value: {'on': true, brightness: 512}}, LED off: {value: {'on': false}}, 'LED on, use last bri"
                           "ghtness': {value: {'on': true}}}}}}, responses: {'200': {description: Success, content: {}}}, security: [{basicAuth: []}"
                           "]}}, /pwr: {get: {tags: [pwr], summary: Get the power value, responses: {'200': {description: Success, content: {applica"
                           "tion/json: {schema: {type: object, properties: {power: {type: boolean}}}, examples: {power off: {value: {power: false}},"
                           " power on: {value: {power: true}}}}}}}, security: [{basicAuth: []}]}}, '/pwr/{value}': {put: {tags: [pwr], summary: Set "
                           "the power on or off, parameters: [{name: value, in: path, required: true, schema: {type: integer}, examples: {power off:"
                           " {value: 0}, power on: {value: 1}}}], responses: {'200': {description: Success, content: {}}}, security: [{basicAuth: []"
                           "}]}}, /status: {get: {tags: [status], summary: Get the board status, responses: {'200': {description: Success, content: "
                           "{application/json: {schema: {$ref: '#/components/schemas/Status'}, examples: {normal operation: {value: {samples: '10967"
                           ",', millis: '17663,', busvoltage: '12.18,', current: '197.03,', maxcurrent: '197.88,', lastMessage: ''}}, short circuit:"
                           " {value: {samples: '36539,', millis: '56638,', busvoltage: '12.21,', current: '6.46,', maxcurrent: '5440,', lastMessage:"
                           " 'Current limit exceeded, current: 5440.00mA'}}}}}}}, security: [{basicAuth: []}]}}, /forcelogin: {get: {tags: [admin], "
                           "summary: Force Login, responses: {'401': {description: Success, content: {}}}}}}, components: {responses: {UnauthorizedE"
                           "rror: {description: Authentication information is missing or invalid, headers: {WWW_Authenticate: {schema: {type: string"
                           "}}}, content: {}}}, securitySchemes: {basicAuth: {type: http, scheme: basic}}, schemas: {Led: {type: object, properties:"
                           " {'on': {type: boolean}, brightness: {type: integer, format: int16}}}, Status: {type: object, properties: {samples: {typ"
                           "e: integer, format: int32}, millis: {type: integer, format: int32}, busvoltage: {type: number, format: float}, current: "
                           "{type: number, format: float}, maxcurrent: {type: number, format: float}, lastMessage: {type: string}}}}}}";

const char * swaggerUI = "<!DOCTYPE html><html lang=\"en\"><head> <meta charset=\"UTF-8\"> <meta name=\"viewport\" ""content=\"width=device-width,"
                         " initial-scale=1.0\"> <meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\"> <script src=\"//unpkg.com/swagger-ui-dis"
                         "t@3/swagger-ui-standalone-preset.js\"></script> <script src=\"//unpkg.com/swagger-ui-dist@3/swagger-ui-bundle.js\"></scr"
                         "ipt> <link rel=\"stylesheet\" href=\"//unpkg.com/swagger-ui-dist@3/swagger-ui.css\"/> <title>Swagger</title></head><body"
                         "> <div id=\"swagger-ui\"></div><script>window.onload=function(){SwaggerUIBundle({url: \"/swagger.yaml\",validatorUrl: nu"
                         "ll, dom_id: '#swagger-ui', presets: [ SwaggerUIBundle.presets.apis, SwaggerUIStandalonePreset], layout: \"StandaloneLayo"
                         "ut\"})}</script></body></html>";

#endif
