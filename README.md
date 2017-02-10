## CS311 Computer Graphics

### 020 Rasterizing Triangle

Just draw triangles.

### 030 Interpolating Triangle

Draw triangles with color interpolated from 3 vertices.

### 040 Texturing

Draw triangle's RGB value base on a texture. i.e. filling the triangle with texture by interpolating texture coordinates and perform lookups in texture file.

### 050-60 Abstracting Interface

Generalize and abstract an universal interface for easier access and call.

### 070-80 Mesh

Drawing meshes (collection of triangles).

### 090 Scene Graph

Draw simple scene graphs. Introduce scene with nodes of meshes.

### 100 Draw 3D Scenes

Introduce 3D, some simple modification based on 2D does the job.

### 110 Draw 3D with Depth in Consideration

Modification on 100 to comply with introduction of the concept of Depth. Apply the Z-buffer to engine.

### 120 Incorporate the concept of camera

Camera activities means inverse of its activities on the model. So just implement an inverse isometry on the model with any camera action.

### 130 Perspective Projection

Implement projection to make the modeling world more realistic and the view point more from a human perspective.

### 140 Clipping

Realistic clipping near the camera. Also avoids fatal `division by 0 errors`.

### 160~190 Lighting

- 160: Add diffuse lighting

  ```c
  /* Lighting 1: diffuse lighting */
  @ d is the diffuse intensity
  R = d * lightR * surfaceR
  G = d * lightG * surfaceG
  B = d * lightB * surfaceB
  where d = max(0, vecDot(vectorNormal, vectorLight))
  ```

- 170: Add specular lighting

  ```c
  /* Lighting 2: specular lighting*/
  ReflectionDir = 2 * vecDot(vecN, vecL) * vecN - vecL
  specInt = max(0, vecDot(cameraDir, ReflectionDir))^shineness
  spec_rgb = sepcInt * LightRGB * ClearCoatRGB(white here)
  rgb = spec_rgb + diffuse_rgb
  ```

- 180: Add ambient lighting

  ```c
  d = max(0, dotProduct); --> d = max(ambientInt, dotProduct); 
  ```

  ​

- 190: Add fog effect

  ```c
  /* Lighting 3: Simple Fog effect*/
  new = (z+1) * worldRGB/2 + (1- (z+1)/2)* FogRGB
  ```

## Moving to Next Stage

### Bugs

- ~~Sphere can be rendered as mesh but rendering it in the scene node throws a bus error.~~ Solved by updating constant `renVERTNUMBOUND` to 1000.
- ~~Rotation is weird, the scene is distorted during rotation.~~ Solved by updating a 0 to 1 in `void mat33AngleAxisRotation`.
- Why somehow changing the projectionType between `perspective` and `orthographic` have weird visual effect. i.e. they have a very different view angle and distance.
- Why `mainFog` not working in `perspective`, only works in `orthographic`.

