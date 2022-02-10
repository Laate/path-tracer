# Path tracer

Work in progress.

Takes as input a description of a 3D scene, simulates how light interacts with the scene and 
produces an image with realistic lighting. Uses Monte Carlo integration to solve the [rendering equation](https://en.wikipedia.org/wiki/Rendering_equation).

**Example usage**:

`./pathtracer --samples 64 --resolution 1280 720 --input ../models/cornell-box.gltf`

`./pathtracer --help` for more options.

## Example renders

<img src="/example-renders/cornell-box.png" width="100%">

cornell-box.gltf

<img src="/example-renders/demo.png" width="100%">

demo.gltf

