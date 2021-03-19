from flask import Flask
from flask import request
from flask import render_template
import pexpect

child = pexpect.spawn('./lightshow')
child.expect('lights count: 3')

app = Flask(__name__)

@app.route('/', methods=['POST', 'GET'])
def index():
    error = None
    if request.method == 'POST':
        data = request.get_json()
        rgb = data['color'].lstrip('#')
        rgb = tuple(int(rgb[i:i+2], 16) for i in (0, 2, 4))
        s_rgb = tuple(str(i) for i in rgb)
        arg = ' '.join(s_rgb*3)
        child.sendline(arg)
        child.expect('Got ')
        return {
            "color": data['color'],
            "rgb": rgb,
            "success": 1,
            "error": error
        }
    elif request.method == 'GET':
        return render_template('index.html')
