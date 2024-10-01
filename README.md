Cornell Box Ray Tracer in C++
-----------------------------

This project implements a basic path tracing ray tracer inside a Cornell box using C++. The ray tracer simulates the physics of light to generate realistic images by tracing the paths of rays as they bounce through the scene, interacting with various objects and light sources.

Features
---------------------
Objects: The scene includes spheres and triangles (used to construct the Cornell box and light sources).

Path Tracing: The ray tracer uses path tracing to simulate global illumination by recursively tracing rays and calculating their interactions with surfaces.

Ambient Occlusion: Simulates soft shadowing in areas where light is partially occluded, like corners and crevices, adding depth and realism to the scene.

Bloom: Adds a glow around bright objects like light sources, simulating the blooming effect that occurs due to light scattering in real-world optics.
