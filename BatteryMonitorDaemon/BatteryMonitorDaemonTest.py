__author__ = 'uk'

import unittest
import datetime
from StringIO import StringIO
from BatteryMonitorDaemon import format_host_command, \
    format_service_command, send_results_to_nagios

class MyTestCase(unittest.TestCase):
    def test_format_host_command(self):

        # aka '1346947461'
        faketime = datetime.datetime(2012, 9, 6, 18, 4, 21, 672588)

        self.assertEqual('[1346947461] PROCESS_HOST_CHECK_RESULT;fake-batt;0;OK - HOST OK',
            format_host_command(faketime, 'fake-batt', 0))

        self.assertEqual('[1346947461] PROCESS_HOST_CHECK_RESULT;fake-batt;1;SHIT - HOST DOWN',
            format_host_command(faketime, 'fake-batt', 1))


    def test_format_service_command(self):
        faketime = datetime.datetime(2012, 9, 6, 18, 4, 21, 672588)
        self.assertEqual("[1346947461] PROCESS_SERVICE_CHECK_RESULT;fake-batt;voltage;0;OK - 12.500 V | voltage=12.500;11.000:;10.000:;0;20.000",
            format_service_command(faketime, 'fake-batt', 0, 12.5))

    def test_send_results_to_nagios(self):
        # Mock the open() statement in the with-clause of write_checksums().
        fake_cmd_file = StringIO()
        faketime = datetime.datetime(2012, 9, 6, 18, 4, 21, 672588)
        send_results_to_nagios('fake-batt', 12.5, fake_cmd_file, faketime)

        self.assertEqual('[1346947461] PROCESS_HOST_CHECK_RESULT;fake-batt;0;OK - HOST OK\n' \
                         '[1346947461] PROCESS_SERVICE_CHECK_RESULT;fake-batt;voltage;0;OK - 12.500 V ' \
                         '| voltage=12.500;11.000:;10.000:;0;20.000\n',
                         fake_cmd_file.getvalue())
        fake_cmd_file.close()

if __name__ == '__main__':
    unittest.main()
