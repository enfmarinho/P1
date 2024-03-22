# Introduction

Implementation of a unix program called cowsay.

# Authors

Name: Caio Luiz Silvério Diógenes.

Email: canalclsd@gmail.com

Name: Eduardo Nestor Ferreira Marinho.

Email: eduardonestormarinho228@gmail.com

# Grading

Item     |  Maximum value | Expected Value
-------- | :-----: | ------------
Read, validate, and process command-line arguments | 20 | 20
Show help as requested `-h` | 2 | 2
Support all cow moods `-bdgpstwy` (8 in total, 2 pts each)  | 16 | 16
Support cow flip `-f`    | 5 | 5
Suspend excessive white space elimination and text wrapping `-n` | 5 | 5
Allow manual definition of eyes with `-E eye_string` | 5 | 5
Allow manual definition of tongue with `-T tongue_string` | 5 | 5
Support definition of wrap column with `-W column_value` | 5 | 5
Read message from standard input correctly | 5 | 5
Remove all excessive white spaces and wrap text at or close to column 40 (default behavior) | 5 | 5
Support message with left-alignment (default behavior, no option needed) | 5 | 5
Support message with center-alignment `-J c` | 5 | 5
Support message with right-alignment `-J r` | 5 | 5
Print billboard decoration (frame) correctly, for single- and multi-line messages | 8 | 8
The project contains at least two classes | 10 | 10
Program handles errors properly | 4 | 4

# Compiling

This project is design to be compiled with Cmake.
To compile this project be sure to be in the main directory, 
called "trabalho-01-cowsay-caio-eduardo" and then run the 
followings commands (assuming '$' is the terminal prompt):

$ Cmake -S source -B build 

$ cd build

$ make

This commands triggers the compiling process.

# Running 
From the "build" directory run (assuming '$' is the terminal prompt):

$ ./cowsay
