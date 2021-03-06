from flask import Flask
from flask import request
from flask import render_template
import pexpect
import sys
import signal
from datetime import datetime, timezone

child = pexpect.spawn('./lightshow')
child.expect('Found Infinity Portal')
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
        now = datetime.now()
        dt = now.astimezone(timezone.utc).strftime('%a %b %d %H:%M:%S %Z %Y')
        f = open('/sys/class/thermal/thermal_zone0/temp', 'r')
        cpu_temp = int(f.readline()) / 1000
        return render_template('index.html', datetime=dt, cpu_temp=cpu_temp)

def lightOff(signum, frame):
    child.sendline('0 0 0 0 0 0 0 0 0')
    print("Lights Off. (" + str(signum) + ") recieved.")
    sys.exit(0)

signal.signal(signal.SIGINT, lightOff)
