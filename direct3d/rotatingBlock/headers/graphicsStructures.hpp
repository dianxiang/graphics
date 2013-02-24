#ifndef _GRAPHICS_STRUCTURE_HPP_
#define _GRAPHICS_STRUCTURE_HPP_

#include <D3dx9math.h>
#include <xnamath.h> 
#include "SystemDefinitions.hpp"

struct Vector3 {
	
	SFLOAT mX;
	SFLOAT mY;
	SFLOAT mZ;

	Vector3(){
		mX = 0.0f;
		mY = 0.0f;
		mZ = 0.0f;
	}

	Vector3( SFLOAT x, SFLOAT y, SFLOAT z ){
		mX = x;
		mY = y;
		mZ = z;
	}
};

struct Vector4 {
	
	SFLOAT mX;
	SFLOAT mY;
	SFLOAT mZ;
	SFLOAT mW;

	Vector4(){
		mX = 0.0f;
		mY = 0.0f;
		mZ = 0.0f;
		mW = 1.0f;
	}

	Vector4( SFLOAT x, SFLOAT y, SFLOAT z, SFLOAT w ){
		mX = x;
		mY = y;
		mZ = z;
		mW = w;
	}
};

struct Vertex {

	Vertex(){
		mPosition = Vector3( 0.0f, 0.0f, 0.0f );	
		mColor = Vector4( 0.0f, 0.0f, 0.0f, 1.0f );
	}

	Vertex( SFLOAT x, SFLOAT y, SFLOAT z ){
		mPosition = Vector3( x, y, z );
		mColor = Vector4( 1.0f, 0.0f, 0.0f, 1.0f );
	}

	Vertex( SFLOAT x, SFLOAT y, SFLOAT z,
			SFLOAT r, SFLOAT g, SFLOAT b, SFLOAT a 
	){
		mPosition = Vector3( x, y, z );
		mColor = Vector4( r, g, b, a );
	}

	Vertex( Vector3 position, Vector4 color
	){
		mPosition = position;
		mColor = color;
	}

	Vector3 mPosition;
	Vector4 mColor;

};

struct ConstantBuffer {
	XMMATRIX rotationMatrix;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;
	XMMATRIX translationMatrix;
};

#endif