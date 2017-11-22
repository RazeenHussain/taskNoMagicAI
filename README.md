# taskNoMagicAI
This repository contains a cpp implementation for computing the distance between cell centers of an arbitrary grid and its corresponding furthest triangle of an object mesh.

Some datasets are included in the task/Dataset directory. The tetrahedron.obj is the simplest file. 
Note: Some datasets may require a lot of computation due to the difference in dimensions of the grid cells and the object dimensions.

For now, the .obj file needs to be changed in the task.cpp file. However, it can be easily modified totake the filename as an argument to the main function by using the argv

The executable uses the OBJ-Loader header file available at https://github.com/Bly7/OBJ-Loader.git to read the .obj files.

The output is stored in the task/out.txt file.
