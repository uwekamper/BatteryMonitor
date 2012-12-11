#!/usr/bin/python

from flask import Flask
from flask.templating import render_template
import datetime

app = Flask(__name__)

VOLTAGE_WARN = 11.0 # [V]
VOLTAGE_CRITICAL = 10.0 # [V]
VOLTAGE_MAX = 15.0 # [V]

# Change this to match your Nagios or Icinga command file.
COMMAND_FILE_NAME = '/usr/local/icinga/var/rw/icinga.cmd'

@app.route('/')
def hello():
    return render_template('index.html')

@app.route('/host/<hostname>/report_voltage/<float:voltage>')
def report_voltage(hostname, voltage):
    try:
        cmd_file = open(COMMAND_FILE_NAME, mode='w')
        send_results_to_nagios(hostname, voltage, cmd_file)
    except Exception, e:
        print str(e)
        abort(500)

    return render_template('thank_you.html')

def send_results_to_nagios(hostname, voltage, cmd_file, time=None):
    if not time:
        time = datetime.datetime.now()

    cmd_file.write(format_host_command(time, hostname, 0) + '\n')

    # Check if the voltages is lower than the critical voltages
    service_state = 0
    if voltage <= VOLTAGE_WARN:
        service_state = 1
    if voltage <= VOLTAGE_CRITICAL:
        service_state = 2
	
	# Send it to Nagios/Icinga
    cmd_file.write(format_service_command(time, hostname, service_state, voltage) + '\n')

def format_host_command(time, hostname, state):
    timestamp = time.strftime("%s")
    state_msg = 'OK - HOST OK'
    if state == 1:
        state_msg = "SHIT - HOST DOWN"
    return '[%s] PROCESS_HOST_CHECK_RESULT;%s;%d;%s' % (timestamp, hostname, state, state_msg)

def format_service_command(time, hostname, state, value):
    timestamp = time.strftime("%s")
    state_msg = 'OK - %.3f V' % value
    if state == 1:
        state_msg = 'WARNING - %.3f V' % value
    if state == 2:
        state_msg = 'CRITICAL - %.3f V' % value

    return '[%s] PROCESS_SERVICE_CHECK_RESULT;%s;voltage;%d;%s | ' \
           'voltage=%.3f;%.3f:;%.3f:;0;%.3f' % (timestamp,
                                              hostname,
											  state,
                                              state_msg,
                                              value,
                                              VOLTAGE_WARN,
                                              VOLTAGE_CRITICAL,
                                              VOLTAGE_MAX)

if __name__ == "__main__":
    app.run(host='0.0.0.0')
