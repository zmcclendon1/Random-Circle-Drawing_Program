#!/bin/bash

gcc particle.c -o particle -I/usr/include/SDL3 -D_REENTRANT -lSDL3 -lm

