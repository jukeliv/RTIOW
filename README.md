# Ray Tracing in One Weekend
 A simple implementation of the book [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) written in C ( as god intended )
 ![](https://github.com/jukeliv/RTIOW/blob/520a024ba5aa7356fa33e357fe374aef16dbe7d5/default_scene_render.png)

# Libraries
 - Arena allocator => The arena allocator is the one in [arena.h](https://github.com/tsoding/arena/blob/master/arena.h) a single-header, stb-like library.
 - Graphics library => All the graphical rendering outside of the actual raytracer is all done using [tigr](https://github.com/erkkah/tigr/), a really simple graphics library that let's you easilly create a window and handle input + utility functions ( all crossplatform ).

# Building
 ```bash
 # Linux
 cc src/*.c -O3 -o rtiow -lGLU -lGL -lX11 -lm -fopenmp
 # Windows
 cc src/*.c -O3 -o rtiow.exe -lopengl32 -lgdi32 -fopenmp
 ```

# Running
 just run `./rtiow`, it optionally can recive a `.png` file where the renderer output will be saved ( it may error ).
 the rendering process takes a while, and when it ends, it opens up a window that shows the render + the time it took to render.

# Fun commit ideas for this repo
 - Add denoising after the render
 - Re-write as a shader
 - Graphically show progress on the render
 - Add GUI for making scenes and then rendering them
 - Save output into a `.png` image
