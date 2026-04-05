# DH2323 — Computer Graphics
> Laboratory work and projects for the **DH2323** course at **KTH Royal Institute of Technology**.  
---

## 📁 Labs Overview

### Lab 1 — Projections & Interpolation
Focused on the transition from 2D pixel plotting to 3D coordinate systems.

- **Linear Interpolation** — Numerical and vector interpolation to generate smooth color gradients across the viewport.
- **Starfield Simulation** — Real-time animation of 3D points moving toward a pinhole camera.
- **Perspective Projection** — Mapping 3D coordinates `(x, y, z)` to 2D screen coordinates `(u, v)` using focal length and aspect ratio calculations.

<p align="center">
  <img src="DH2323Lab1 v1.2/DH2323Lab1/task1/task1_summary.png" width="45%" />
  <img src="DH2323Lab1 v1.2/DH2323Lab1/task3/starfield.gif" width="45%" />
</p>

---

### Lab 2 — Raytracing
A CPU-based raytracer designed to render 3D scenes by tracing light paths from the camera into the environment.

- **Geometry Engine** — Scenes modeled using triangular surfaces with pre-computed normals.
- **Ray-Triangle Intersection** — Solving the linear system `Ax = b` via matrix inversion to find the exact hit point `(t, u, v)`.
- **Camera System** — Interactive movement with `3×3` rotation matrices for Y-axis yaw.
- **Lighting Model:**
  - *Direct Illumination* — Lambertian reflectance based on the dot product between surface normals and light direction vectors.
  - *Shadow Rays* — Secondary rays cast from intersection points to the light source to detect occlusions.
  - *Indirect Illumination* — Constant ambient term approximation to simulate light bounces.

<p align="center">
  <img src="DH2323Lab2/task5/rotation.png" width="31%" />
  <img src="DH2323Lab2/task6/directBW.png" width="31%" />
  <img src="DH2323Lab2/task6/indirectLight.png" width="31%" />
</p>

### Lab 3 — Rasterization
Focus on real-time rendering using rasterization, transitioning from ray-based rendering to scanline-based triangle filling.

- **Wireframe Rendering** — Projection of 3D triangles onto screen space for structural validation.
- **Triangle Rasterization** — Filling triangles using row-wise interpolation between edges.
- **Depth Buffer (Z-buffer)** — Visibility handling using inverse depth (`1/z`) comparisons.
- **Shading Models:**
  - *Per-Vertex Illumination* — Lighting computed at vertices and interpolated.
  - *Per-Pixel Illumination* — Lighting computed per fragment for higher accuracy.
- **Perspective-Correct Interpolation** — Attributes interpolated using `1/z` to avoid distortion caused by perspective projection.

<p align="center">
  <img src="DH2323Lab3/task4/DrawLine.png" width="31%" />
  <img src="DH2323Lab3/task6/correct_output.png" width="31%" />
  <img src="DH2323Lab3/task7/illPerPixel(perspectiveInterp).png" width="31%" />
</p>

---
