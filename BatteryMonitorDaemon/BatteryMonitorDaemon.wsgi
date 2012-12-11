#!/usr/bin/python

activate_this = '/home/uk/BatteryMonitorDaemon/bin/activate_this.py'
execfile(activate_this, dict(__file__=activate_this))

# add current directory to search path for python modules
import site
site.addsitedir('/home/uk/BatteryMonitorDaemon/')

from BatteryMonitorDaemon import app as application
