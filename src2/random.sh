#!/bin/bash

ttab "python ../Tak-sim/server.py 1702"
ttab "python ../Tak-sim/client.py 127.0.0.1 1702 run2.sh"
ttab "python ../Tak-sim/client.py 127.0.0.1 1702 -mode CUI run2.sh"