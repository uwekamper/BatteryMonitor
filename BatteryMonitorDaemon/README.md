# Installation instructions # 

virtualenv --distribute --no-site-packages .
source bin/activate
pip install -r requirements.txt

chmod :www-data *.py
chmod :www-data *.wsgi
chmod +x *.py
chmod +x *.wsgi

see apache-cfg-example.txt for the Apache2 WSGI configuration.
