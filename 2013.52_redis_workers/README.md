2013 w52: Redis Workers
=======================
Workers generate pdfs in a result directory using user data.
Workers run as daemons. Server can be blocking cli; write user data,
read file path.

Tech
----
- C (Server)
- Ruby (Workers)
- CMake
- Redis

How to run
----------
First start redis (redis-server).
Then start a worker or two:

```shell
./worker.rb
```
or
```shell
god -c worker.god -D
```

Then build and run the producer:
```shell
cmake . && make && ./RedisMailGenerator
```
Notes
-----
PDF generating hasn't been implemented yet.
Also skipped subscribing to jobs finished.
No expiration on keys.

![Screenshot](https://raw.github.com/peteb/katas/master/2013.52_redis_workers/screenshot.png)
