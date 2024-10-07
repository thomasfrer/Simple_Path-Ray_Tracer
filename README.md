Cornell Box Ray Tracer in C++
-----------------------------

This project is a basic path-tracing ray tracer inside a Cornell box using C++. The ray tracer simulates the physics of light to generate a realistic image by tracing the paths of rays as they bounce through the scene, interacting with various objects and light sources. It takes some time to accumulate, so give it some time to run for the best results. 

Features
---------------------
- Objects: The scene includes spheres and triangles (used to construct the Cornell box and light sources).

- Path Tracing: The ray tracer uses path tracing to simulate global illumination by recursively tracing rays and calculating their interactions with surfaces.

- Ambient Occlusion: Simulates soft shadowing in areas where light is partially occluded, like corners and crevices, adding depth and realism to the scene.

- Bloom: Adds a glow around bright objects like light sources, simulating the blooming effect that occurs due to light scattering in real-world optics.

Changelog
---------------------
v1.1
- Significantly improved trace speed by adding OpenMP Multithreading.

- Improved scene quality by implementing Importance Sampling.

v1.0
- Initial Release
---------------------
![full accumulation](https://github.com/user-attachments/assets/e9c56272-8749-4c65-9e74-605f79a67842)
