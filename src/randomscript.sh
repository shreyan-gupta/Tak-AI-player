#!/bin/bash

# ttab "echo 1234"
ttab "python server.py 10000"
ttab "python client.py 127.0.0.1 10000 RandomPlayer.py"
ttab "python client.py 127.0.0.1 10000 RandomPlayer.py -mode CUI"