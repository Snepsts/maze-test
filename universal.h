/* maze-test
Copyright (C) 2017 Michael Ranciglio

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef __UNIVERSAL_H__
#define __UNIVERSAL_H__

#include <random> //default_random_engine
#include <ctime> //time(nullptr) (for default_random_engine)

/* The current purpose of this file is to have one default random engine across
all functions of the maze. Since my game also has a universal.h I thought this
would be a good idea for consistency since I will likely be using one
default_random_engine across the entire project. This fixed a lot of problems I
was experiencing with random number generation being... not so random. */

std::default_random_engine randmaze(time(nullptr));

#endif
