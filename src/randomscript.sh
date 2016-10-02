#!/bin/bash

ttab "python server.py 10002"
ttab "python client.py 127.0.0.1 10002 out1.sh"
ttab "python client.py 127.0.0.1 10002 -mode CUI out2.sh"