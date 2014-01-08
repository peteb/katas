2013 w52: Redis Workers
=======================
Workers generate pdfs in a result directory using user data.
Workers run as daemons. Server can be blocking cli; write user data,
read file path.

Tech
----
C (Server), Ruby (Workers), CMake, Redis, LaTeX/pdflatex (Generating PDF)
XXXX (Testing server - worker integration)

How to run
----------
First start redis; redis-server.

./worker.rb
or
god -c worker.god -D

cmake .
./RedisMailGenerator

Notes
-----
PDF generating hasn't been implemented yet.
Also skipped subscribing to jobs finished.
No expiration on keys.

![Screenshot](https://raw.github.com/peteb/katas/master/2013.52_redis_workers/screenshot.png)