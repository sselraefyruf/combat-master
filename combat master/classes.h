#pragma once

#include "def.h"

class pCamera {
public:
	char _0x0000[0x2E4]; //740
	matrix<float> view_matrix; // 0x02E4
};

class Camera
{
public:
	char _0x000[0x10];
	pCamera* m_cashedptr;
};