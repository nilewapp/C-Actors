#!/usr/bin/env python
import sys
import urllib.request as RE

requests = int(sys.argv[1])
url = sys.argv[2]

# Send *many* http requests
for i in range(1, requests):
    response = RE.urlopen(url)
