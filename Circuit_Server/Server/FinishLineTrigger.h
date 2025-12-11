#pragma once

class FinishLineTrigger {
public:
    float xMin, xMax;
    float zMin, zMax;

    FinishLineTrigger()
        : xMin(0), xMax(0), zMin(0), zMax(0)
    {
    }

    FinishLineTrigger(float xmin, float xmax, float zmin, float zmax)
        : xMin(xmin), xMax(xmax), zMin(zmin), zMax(zmax)
    {
    }

    bool IsInside(float x, float z) const {
        return (x >= xMin && x <= xMax &&
            z >= zMin && z <= zMax);
    }
};
