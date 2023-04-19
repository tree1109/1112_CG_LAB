#pragma once
#include "myObject.h"

enum class OBJECT {
    TEAPOT,
    TEDDY,
    OCTAHEDRON,
    GOURD
};

extern OBJECT currentObject;
extern RENDER_MODE currentRenderMode;
extern COLOR_MODE currentColorMode;
