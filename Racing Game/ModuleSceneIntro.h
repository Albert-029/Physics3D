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

struct BodyRotation {
	float angle;
	vec3 axis_pos;
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

	void CreateWorld();

public:

	SDL_Rect timer, time_left;
	SDL_Texture* graphics = nullptr;
	int time_toMove = 2000;

	Timer clock;

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

	p2DynArray<Cube*> FinishCubes;
	p2DynArray<PhysBody3D*> bodyFinishCubes;

private:
	void	LimitsRender();
	int		num_limits;

	void	FloorsRender();
	int		num_floors;

	void	RoadRender();
	int		num_roads;

	void	FinishRender();
	int		num_finish;
};
