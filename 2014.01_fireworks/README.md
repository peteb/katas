2014 w01: Fireworks
=======================
Let the user launch some fireworks to celebrate the new year!

Tech
----
- C++
- OpenGL
- GLFW
- CMake

(SFML was decided against, problems with OpenGL 3.2 core there...)

How to run
----------
```shell
cmake . && make
```

Notes
-----
I couldn't let this project slip without making it look a bit better.

I wanted to have textured particles but it took too much time; partly due to me not having worked with it for some years. I tried to use the CImg image loading library to load raw image data, but that was a failure. Turned out it didn't understand my jpg. The dependency on X11 and slow compilation time (everything's in one header file!) wasn't very good either. I would've tried DevIL if it wasn't for the gcc requirement. Agh, image loading is always a big mess.

Screenshot
----------
![ScreenShot](https://raw.github.com/peteb/katas/master/2014.01_fireworks/screenshot.png)
