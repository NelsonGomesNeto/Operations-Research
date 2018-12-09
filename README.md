# Operations Research
## Here I will model and "implement" some [linear optimization problems](https://1drv.ms/f/s!Av7ZeqaYG5k95u4Wd5oC7LGhRU8f-g) using CPlex library for C++ ^~^

## Setup instructions:
* [Download CPlex](https://www.ibm.com/developerworks/br/downloads/ws/ilogcplex/index.html)
* ### Windows:
  * IBM made it specially for Visual Studio, and I couldn't be able to port it to anywhere (and it doesn't seem to be possible); it seems to be easy to setup it on VS, but I didn't try it. But you can run it with WSL (Windows Subsystem for Linux) and configure include folder on any IDE, so you will get some lints; and that's what I did.
* ### Linux:
  1. Make sure to have JDK 8 installed (If it still doesn't work, add it to the PATH).
      1. sudo apt install openjdk-8-jdk
  2. With the .bin (I will call it cplex.bin) downloaded, do:
      1. chmod +x cplex.bin
      2. ./cplex.bin
      3. Follow the installer. You can install it in any folder you want, just remember the path of it, we will need it to adjust the Makefile.
  * Use the Makefile to compile:
      * It receives: "path=\<path to the name of your file (without .cpp)\>" as argument, so you don't need to edit everytime neither copy it to the folder you're working on.
        * Example: make TransportProblem/transportProblem
      * If there's no compilation error: it will generate a yourFile.run
  * Run it using: ./yourFile.run
* ### Lint/Autocomplete:
  * In order to use lint/autocomplete in any IDE, you need to provide the "include" folder of the library you will use. But, for some reason, CPLEX include had a small bug that broke VisualCode's lint (and probably any other IDE). I fixed and it's [here](https://github.com/NelsonGomesNeto/Operations-Research/Fixed%include). Just copy and paste it into the "include" folder of your compiler.