# Introduction

This project is a terminal version of a bar chart animation.

# Author(s)

- Authors: *Caio Luiz Silvério Diógenes* e *Eduardo Nestor Ferreira Marinho*
- Emails: *<canalclsd@gmail.com>* e *<eduardonestormarinho228@gmail.com>*

# Grading

Item     | Valor máximo   | Valor esperado
-------- | :-----: | :-----:
Read, validate, and process command line arguments | 5 | 5
Program runs with default values, if no options are provided | 5 | 5
Read the input data file and store the information in memory    | 10 | 10
Display a summary of the data read from the input file *before* the animation starts | 5 | 5
Display a summary of the running options (set by the user or default values) *before* the animation starts | 5 | 5
The project contains at least two classes | 10 | 10
Create individual bar charts containing all data items for a single date | 10 | 10
Run the bar chart animation at the correct speed | 10  | 10
The bar chart anatomy is complete and correct | 30 | 30
Program handles errors properly | 10 | 10

# Compiling

This project is design to be compiled with Cmake.
To compile this project be sure to be in the main directory,
called "trabalho-03-barchart-race-caio-eduardo-1" and then run the
followings commands (assuming '$' is the terminal prompt):

$ Cmake -S source -B build

$ cd build

$ make

This commands triggers the compiling process.

# Running

From the "build" directory run (assuming '$' is the terminal prompt):

$ ./bcr
