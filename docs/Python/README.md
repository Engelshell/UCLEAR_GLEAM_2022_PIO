

# Python3 Dev Environment Setup

Download and install python https://www.python.org/downloads/

* In the installer make sure `"Add python to environment variables"` is checked.
This allows you to use the "python" program in powershell or other command lines

* Customize Install Location to `C:\Python310\`


## Create Python Virtual Environment
Different python projects can have different conflicting packages, configuration,
and versions. A "virtual environment" isolates this to your specific project, preventing
problems down the road. Each python project you create or use has to have its own virtual environment.

Open Powershell or other terminal. Here is the command to create a virtual environment in a directory:

`python -m venv "C:\Users\enge0605\Documents\RoverPython\env"`

Replace the path with your own path of course, keeping the env folder name

### Activate Virtual Environment

You have to run a script to make your current terminal session have the correct environment variables. If you're trying to run your program and are missing packages or odd things it's probably because you forgot to activate your virtual environment.
`.\env\Scripts\activate`


### Installing Packages

`py -m pip install cobs`


