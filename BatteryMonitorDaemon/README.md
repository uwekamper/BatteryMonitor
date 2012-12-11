Installation instructions
=========================

This is the Python server part (it is using the Flask micro-framework) of the Battery Monitor. 
You need an Apache2 with mod_wsgi (something like Gunicorn/Nginx should work too, but there are 
no examples included).

    virtualenv --distribute --no-site-packages .
    source bin/activate
    pip install -r requirements.txt

    chmod :www-data *.py
    chmod :www-data *.wsgi
    chmod +x *.py
    chmod +x *.wsgi

See apache-cfg-example.txt for the Apache2 WSGI configuration.
