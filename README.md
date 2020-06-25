# Lab-5

Installation on Windows:
========================
It is recommended to use an IDE (CLion, Visual Studio, ...) but everything can be done without as well  

To use the Framework using Visual Studio or CLion you can follow [this](https://www.youtube.com/watch?v=AjvwygYQT1k&feature=youtu.be) video.  

**Requirements**:  
- [git](https://gitforwindows.org/)
- [cmake](https://cmake.org/download/)
- [make for windows](http://gnuwin32.sourceforge.net/packages/make.htm)  
- compiler and cmake generator i.e. [MinGW-w64](https://sourceforge.net/projects/mingw-w64/)  
  
**Note**: One way to install MinGW 64:
- install [MSYS2](https://www.msys2.org/)  
- open the newly installed MSYS2 Shell
- install MinGW 64 using the MSYS2 Shell  
```
pacman -S mingw-w64-x86_64-gcc
```  
- add the folder MSYS2_root/mingw64/bin (i.e. C:\msys64\mingw64\bin) to your PATH environment variable  
   
You can also follow this [tutorial](https://www.youtube.com/watch?v=f3Ion00p78M) to install MinGW 64.  
  
**Step 0**: open the windows console (cmd) and go into the folder where the project files should be saved
  
**Step 1**: clone the repository 
```
git clone --recursive https://github.com/Realtime-Rendering-II/Lab_5.git
```  
**Note**: this will copy the necessary project files into the current directory.    
  
**Step 2**: create a build folder in the project folder
```
cd Lab_5
mkdir build
```  
**Step 3**: open cmake-gui:  
![Cmake](https://www.uni-weimar.de/~bexo5814/rrII/images/CMake-0.png) 
  
  select the source (C:\\..\Lab_5) and build folder (C:\\..\Lab_5\build)    
  click "generate" and select the cmake generator  
  (in this case MinGW Makefiles is chosen)  
  **Note**: make sure the environment variable for the generator is set  
  
  If the generation was successful it should look like this:  
  ![Cmake](https://www.uni-weimar.de/~bexo5814/rrII/images/CMake-1.png) 

**Step4**: go into the build folder and make the project  
```
cd build
make
```
If the make command was executed successfully it should look like this:  
  ![Make](https://www.uni-weimar.de/~bexo5814/rrII/images/Make.png)   
  
**Step 5**: execute the application:
```
.\main.exe
```

**Note**: If your project is build in a different folder you have to take care of the shader-paths  
  
  
Installation on Linux:
========================
It is recommended to use an IDE (CLion, Visual Studio, ...) but everything can be done without as well  

**Step 0:** open a terminal  
  
**Step 1**: install dependencies  
```
sudo apt-get update
sudo apt-get install git cmake libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libglu1-mesa-dev
```

**Step 2**: clone the repository 
```
git clone --recursive https://github.com/Realtime-Rendering-II/Lab_5.git
```

**Step 3**: create a build folder in the project folder
```
cd Lab_5
mkdir build
```
**Step 4**: inside the build folder generate the cmake project
```
cd build
cmake ..
```
**Step 5**: make the project and execute the application
```
make
./main
```  

Video:
========================
You can see the edge collapse in action [here](https://www.youtube.com/watch?v=bq0TeIlSd6E)

Images:
========================
Edge collapses visualised:  

![Cmake](https://www.uni-weimar.de/~bexo5814/rrII/images/bunny.gif)
  
Cube 0 edge collapses:  
![Cmake](https://www.uni-weimar.de/~bexo5814/rrII/images/cube_1.jpg)  

Cube 1 edge collaps:  
![Cmake](https://www.uni-weimar.de/~bexo5814/rrII/images/cube_2.jpg)  

Cube 2 edge collapses:  
![Cmake](https://www.uni-weimar.de/~bexo5814/rrII/images/cube_3.jpg)  

Cube 3 edge collapses:  
![Cmake](https://www.uni-weimar.de/~bexo5814/rrII/images/cube_4.jpg) 

Cube 4 edge collapses:  
![Cmake](https://www.uni-weimar.de/~bexo5814/rrII/images/cube_5.jpg)  

Cube 5 edge collapses:  
![Cmake](https://www.uni-weimar.de/~bexo5814/rrII/images/cube_6.jpg)  
