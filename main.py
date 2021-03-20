from flask import Flask
from flask import request
from flask import render_template
import pexpect

child = pexpect.spawn('./lightshow')
child.expect('lights count: 3')
lights = [(255, 0, 0), (0, 255, 0), (0, 0, 255)]

app = Flask(__name__)

@app.route('/', methods=['POST', 'GET'])
def index():
    global lights
    error = None
    if request.method == 'POST':
        data = request.get_json()
        lightId = data['lightId']
        rgb = data['color'].lstrip('#')
        rgb = tuple(int(rgb[i:i+2], 16) for i in (0, 2, 4))
        if lightId == 0:  # lightId: 0 means All Lights
            lights = [rgb, rgb, rgb]
            s_rgb = tuple(str(i) for i in rgb)
            arg = ' '.join(s_rgb*3)
        else:
            lights[lightId - 1] = rgb
            all_rgb = lights[0] + lights[1] + lights[2]
            s_rgb = tuple(str(i) for i in all_rgb)
            arg = ' '.join(s_rgb)
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
