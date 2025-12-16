# Flask server for paywall application
Server contains endpoints for create, activate and check license key, db models and rsa signing system.
## Usage
```
python -m venv .venv 
pip install -r .\requirements.txt 
python app.py
```
## API

- [**GET** /generate-key](#generate-key)
- [**POST** /activate-key](#activate-key)
- [**GET** /get-signature/\<hwid\>](#get-signature)

- [Error response format](#error-format)
<p id='generate-key'></p>

#### GET /generate-key
Returns html page with new license key value.

**Responses:**
- **200** - OK.
- **500** - Internal server error (something is broken).


<p id='activate-key'></p>

#### POST /activate-key
Should contain payload:
```
"base64-encoded-json-request-string"
```
Json request:
```
{
    "hwid": "16-chars-hwid",
    "key": "license-key-from-key-generator"
}
```
Connects device and license key. It is assumed that now device has activated license. Returns rsa signing from hwid if device has no activated license key and specified license key was not activated.

**Responses:**
- **200** - OK.
- **400** - If json-request is invalid.
- **404** - If license key not found in db.
- **409** - If device has activated license key or specified license key is activated.
- **500** - Internal server error (something is broken).

**200 - OK:**
```
"base64-encoded-json-reply-string"
```
Json reply:
```
{
    "signature": "base64-encoded-signature-from-hwid-string"
}
```


<p id='get-signature'></p>

#### GET /get-signature/\<hwid\>
\<hwid\> is unique device identifier of length 16.
Returns rsa signing from hwid if device has activated license key.

**Responses:**
- **200** - OK.
- **401** - If device found in db, but doesn't have activated license key.
- **404** - If device not found in db.
- **500** - Internal server error (something is broken).

**200 - OK:**
```
"base64-encoded-json-reply-string"
```
Json reply:
```
{
    "signature": "base64-encoded-signature-from-hwid-string"
}
```


<p id='error-format'></p>

#### Error response format
```
"base64-encoded-error-json-reply-string"
```
Json reply:
```
{
    "error": "error-description-string"
}
```