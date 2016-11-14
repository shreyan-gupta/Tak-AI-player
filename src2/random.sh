#!/bin/bash

ttab "python ../Tak-sim/server.py 1702 -n 6 -TL 180"
ttab "python ../Tak-sim/client.py 127.0.0.1 1702 run1.sh -n 6"
ttab "python ../Tak-sim/client.py 127.0.0.1 1702 -mode CUI run2.sh -n 6"