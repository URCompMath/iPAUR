# iPAUR
### (image Processing Algorithms at University of Regensburg)

## Implemented Algorithms and Basic Image Processing

* Serial algorithms:
  * ROF-Model
  * TV-L1-Model
  * Image Inpainting
  * Huber-ROF-Model
  * Real-Time Minimizer for the Mumford-Shah functional
* Other basic serial algorithms:
  * mean value blur
  * gaussian blur
  * canny edge detection
  * dilatation
  * erosion
  * duto filter
  * gradient filter (Sobel, Prewitt, Robert's Cross)
  * color space conversions
  * inverse image
  * laplace filter (operator)
  * median filter
* Parallel algorithms:
  * Primal-Dual Algorithm to solve the convex relaxed Mumford-Shah functional using Dykstra's projection algorithm
  * Primal-Dual Algorithm to solve the convex relaxed Mumford-Shah functional using an approach with Lagrange multipliers

## Requirements

### CUDA

For the use of the GPU implementation, [CUDA](https://developer.nvidia.com/cuda-downloads) is needed.

### OpenCV

For reading and writing images [OpenCV](http://opencv.org/downloads.html) is needed.