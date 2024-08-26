<h1>MPI Image Scaling Tool</h1>
For NVIDIA CUDA implementation check out [this repository](https://github.com/denis-the-menace/cuda-image-scaling).

<h2>How to compile and run</h2>
Install MPI C compiler for your system (openmpi package in most distributions).<br>
<code>make</code><br>
<code>mpirun -n [number_of_cores] [executable_filename] [image_filename] [output_filename] [scale_factor_x] [scale_factor_y]</code>
