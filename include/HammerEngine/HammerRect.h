/*
 * Copyright (c) 2026 MidnightHammer-code
 * This source code is licensed under the GPL 3.0 license
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HAMMERRECT_H
#define HAMMERRECT_H

#include <vector>

class HammerRectCube{
public:
	int x;
	int y;
	int z;

	int w;
	int h;
	int d;

	bool HammerRectCollideCube(HammerRectCube rect);
	int HammerRectCollideFaceCube(HammerRectCube rect);
	HammerRectCube(int X, int Y, int Z, int W, int H, int D);
};


class HammerRectCubeF{
public:
	float x;
	float y;
	float z;

	float w;
	float h;
	float d;
	bool HammerRectCollideCubeF(HammerRectCubeF rect);
	int HammerRectCollideFaceCubeF(HammerRectCubeF rect);
	HammerRectCubeF(float X, float Y, float Z, float W, float H, float D);
};



class HammerRectSquare{
public:
	int x;
	int y;

	int w;
	int h;
	bool HammerRectCollideSquare(HammerRectSquare rect);
	int HammerRectCollideFaceSquare(HammerRectSquare rect);
	HammerRectSquare(int X, int Y, int W, int H);
};

class HammerRectSquareF{
public:
	float x;
	float y;

	float w;
	float h;
	bool HammerRectCollideSquareF(HammerRectSquareF rect);
	int HammerRectCollideFaceSquareF(HammerRectSquareF rect);
	HammerRectSquareF(float X, float Y, float W, float H);
};


// finish the Cercle to Square and cube

class HammerRectCercle{
public:
	int x;
	int y;

	int r;

	bool HammerRectCollideCercleToCercle(HammerRectCercle rect);
	HammerRectCercle(int X, int Y, int R);
};

class HammerRectCercleF{
public:
	float x;
	float y;

	float r;

	bool HammerRectCollideCercleToCercleF(HammerRectCercleF rect);
	HammerRectCercleF(float X, float Y, float R);
};

///

class HammerRectSphere{
public:
	int x;
	int y;
	int z;

	int r;

	bool HammerRectCollideSphereToSphere(HammerRectSphere rect);
	HammerRectSphere(int X, int Y, int Z, int R);
};

class HammerRectSphereF{
public:
	float x;
	float y;
	float z;

	float r;

	bool HammerRectCollideSphereToSphereF(HammerRectSphereF rect);
	HammerRectSphereF(float X, float Y, float Z, float R);
};

#endif
