#!/bin/bash

problem=4A
grader_name=grader

g++ -std=gnu++17 -O2 -Wall -o "${problem}" "${grader_name}.cpp" "${problem}.cpp"
