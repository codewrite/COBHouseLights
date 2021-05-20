from flask_restplus import Api

from .led import api as led

api = Api(
    title='COBLights API',
    version='0.1',
    description='Raspberry Pi COB Lights API',
    # All API metadatas
)

api.add_namespace(led)
# ...
