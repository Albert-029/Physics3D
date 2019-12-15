#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;

struct BodySize {
	float sizeX;
	float sizeY;
	float sizeZ;
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

public:

	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	p2DynArray<Cube*> FloorCubes;
	p2DynArray<PhysBody3D*> bodyFloorCubes;

	p2DynArray<Cube*> LimitCubes;
	p2DynArray<PhysBody3D*> bodyLimitCubes;

	p2DynArray<Cube*> RoadCubes;
	p2DynArray<PhysBody3D*> bodyRoadCubes;

	p2DynArray<Cube*> DetailsCubes;
	p2DynArray<PhysBody3D*> bodyDetailsCubes;

	p2DynArray<Cube*> PeopleCubes;
	p2DynArray<PhysBody3D*> bodyPeopleCubes;

	p2DynArray<Cube*> BuildingsCubes;
	p2DynArray<PhysBody3D*> bodyBuildingsCubes;

private:
	void	FloorsRender();
	int		num_floors;
};
