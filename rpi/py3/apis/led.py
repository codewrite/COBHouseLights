from io import BytesIO
from datetime import datetime
from flask import Response, make_response, send_file
from flask_restplus import Namespace, Resource

api = Namespace('led', description='LED controls')

# Helper methods

# API methods

@api.route('/led/')
class LedHelper(Resource):
  def get(self):
    return ''
