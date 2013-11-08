#!/bin/bash

mpiexec -n $(( 1+3+2 )) ./floyd-parallel
