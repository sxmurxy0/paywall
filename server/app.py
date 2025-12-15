from flask import Flask, request, render_template, abort
from werkzeug.exceptions import HTTPException
from http import HTTPStatus
import json, base64, secrets
from models import database, Device, Key
import rsa_utils


app = Flask(__name__)

app.config.from_file('config.json', json.load)
database.init_app(app)

private_key = rsa_utils.load_private_key('keys/private.pem')

def signature_response(hwid):
    response = {
        'signature': rsa_utils.sign_data(hwid, private_key)
    }
    data = json.dumps(response).encode()

    return base64.b64encode(data)

@app.errorhandler(HTTPException)
def handle_http_exception(ex):
    response = {
        'error': ex.description
    }
    data = json.dumps(response).encode()

    return base64.b64encode(data), ex.code

@app.get('/')
def ping():
    return "", 200

@app.get('/generate-key')
def generate_key():
    key_value = secrets.token_hex(16)

    key = Key(key_value)
    database.session.add(key)

    database.session.commit()
    
    return render_template('key_generator.html', key=key_value)

@app.post('/activate-key')
def activate_key():
    decoded_str = base64.b64decode(request.get_data())
    payload = json.loads(decoded_str)

    hwid = payload.get('hwid')
    key_value = payload.get('key')

    if key_value is None or hwid is None:
        abort(HTTPStatus.BAD_REQUEST, description='Invalid payload, <key> or <hwid> not specified!')

    key = database.session.query(Key).where(Key.value == key_value).scalar()
    if key is None:
        abort(HTTPStatus.NOT_FOUND, description='License key not found!')

    if key.device is not None:
        abort(HTTPStatus.CONFLICT, description='License key already activated!')
    
    device = database.session.query(Device).where(Device.hwid == hwid).scalar()
    if device is None:
        device = Device(hwid)
        database.session.add(device)
    
    if device.key is not None:
        abort(HTTPStatus.CONFLICT, description='Device already has activated license key!')

    key.device = device

    database.session.commit()

    return signature_response(hwid)

@app.get('/get-signature/<hwid>')
def get_signature(hwid):
    device = database.session.query(Device).where(Device.hwid == hwid).scalar()
    
    if device is None:
        abort(HTTPStatus.NOT_FOUND, description='Device not found!')

    if device.key is None:
        abort(HTTPStatus.UNAUTHORIZED, description='Device doesn\'t have activated license key!')

    return signature_response(hwid)

if __name__ == '__main__':
    with app.app_context():
        database.create_all()
    
    app.run()