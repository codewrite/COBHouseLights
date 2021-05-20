#!/usr/bin/env python3
from flask import Flask
from flask_cors import CORS
from apis import api

# App definition
app = Flask(__name__)
CORS(app)
api.init_app(app)

# *******************************************************************
# Main App
# *******************************************************************

if __name__ == '__main__':
    # certs copied from /etc/ssl/mycerts
    print('running...')
    app.run(host='0.0.0.0', debug=True, ssl_context=('certs/nginx.pem', 'certs/nginx.key'), port=5443)
    # app.run(host='0.0.0.0', debug=False)
    print('stopping...')
