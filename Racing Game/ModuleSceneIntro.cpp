#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	CreateWorld();

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	FloorCubes.Clear();
	RoadCubes.Clear();
	FinishCubes.Clear();
	LimitCubes.Clear();

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	for (int i = 0; i < num_floors; i++)
	{
		FloorCubes[i]->Render();
	}

	for (int i = 0; i < num_limits; i++)
	{
		LimitCubes[i]->Render();
	}

	for (int i = 0; i < num_roads; i++)
	{
		RoadCubes[i]->Render();
	}

	for (int i = 0; i < num_finish; i++)
	{
		FinishCubes[i]->Render();
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

void ModuleSceneIntro::CreateWorld()
{
	//Rendering floor and sky

	FloorsRender();

	// Rendering map limits

	LimitsRender();

	// Rendering the road borders

	RoadRender();

	FinishRender();
}

void ModuleSceneIntro::FloorsRender()
{
	num_floors = 2;

	BodySize FloorSize[2] = { {800, 1, 800}, {800, 1, 800} };

	vec3 FloorPosition[2] = { {0, 0, 0}, {0, 80, 0} };

	for (int i = 0; i < num_floors; i++)
	{
		FloorCubes.PushBack(new Cube(FloorSize[i].sizeX, FloorSize[i].sizeY, FloorSize[i].sizeZ));
		FloorCubes[i]->SetPos(FloorPosition[i].x, FloorPosition[i].y, FloorPosition[i].z);
		if (i == 0) { FloorCubes[i]->color = Grey2; }
		else if (i == 1) { FloorCubes[i]->color = Blue2; }
		bodyFloorCubes.PushBack(App->physics->AddBody(*(FloorCubes[i]), 0.0f));
	}
}

void ModuleSceneIntro::LimitsRender()
{
	num_limits = 4;

	BodySize LimitsSize[4] = { {2, 130, 800}, {2, 130, 800}, {800, 130 , 2}, {800, 130, 2} };
	vec3 LimitsPosition[4] = { {-200, 15, 0}, {200, 15, 0}, {0, 15, 200}, {0, 15, -200} };

	for (int j = 0; j < num_limits; j++)
	{
		LimitCubes.PushBack(new Cube(LimitsSize[j].sizeX, LimitsSize[j].sizeY, LimitsSize[j].sizeZ));
		LimitCubes[j]->SetPos(LimitsPosition[j].x, LimitsPosition[j].y, LimitsPosition[j].z);
		LimitCubes[j]->color = Blue2;
		bodyLimitCubes.PushBack(App->physics->AddBody(*(LimitCubes[j]), 0.0f));
	}
}

void ModuleSceneIntro::FinishRender()
{
	num_finish = 3;
	BodyRotation no_rotation = { 0,{0,1,0} };

	BodySize FinishSize[3] = { {2,10,2}, {2,10,2}, {18,3.3,2}};
	vec3 FinishPosition[3] = { {390,10,15}, {370,10,15}, {380,13,15}};
	BodyRotation FinishRotation[74] = { no_rotation, no_rotation, no_rotation};

	for (int i = 0; i < num_finish; i++)
	{
		FinishCubes.PushBack(new Cube(FinishSize[i].sizeX, FinishSize[i].sizeY, FinishSize[i].sizeZ));
		FinishCubes[i]->SetPos(FinishPosition[i].x, FinishPosition[i].y, FinishPosition[i].z);
		FinishCubes[i]->SetRotation(FinishRotation[i].angle, FinishRotation[i].axis_pos);
		if (i == 0 || i == 1)
		{
			FinishCubes[i]->color = White;
		}
		else if (i == 2)
		{
			FinishCubes[i]->color = Grey;
		}
		else if (i > 2 && i < 47)
		{
			FinishCubes[i]->color = Black;
		}
		bodyFinishCubes.PushBack(App->physics->AddBody(*(FinishCubes[i]), 0.0f));
	}
}

void ModuleSceneIntro::RoadRender()
{
	num_roads = 62;

	BodySize curva_oberta = { 2,5,50 };
	BodySize curva_tancada = { 2,5,25 };

	//						  //   	recta 1       //          curva 1           //     recta 2     //	  recta 3	   //	   recta 4	    //	    recta 5	     //		  recta 6	   //	   recta 7	   //	   curva 2      //	      curva 3    //	     recta 8	 //	      recta 9	   // 
	BodySize RoadSize[62] = { {2,5,220}, {2,5,190}, curva_oberta, curva_tancada, {2,5,50}, {2,5,70}, {2,5,50}, {2,5,50}, {2,5,100}, {2,5,50}, {2,5,100}, {2,5,60}, {2,5,120}, {2,5,120}, {2,5,20}, {2,5,60}, {2,5,140}, {2,5,140}, {2,5,85}, {2,5,85}, {2,5,60}, {2,5,110}, {2,5,100}, {2,5,150} };
	//                        //      recta 1         //         curva 1            //          recta 2         //		     recta 3	   	    //			recta 4			  //			recta 5			//			recta 6		     //			 recta 7		  //		  curva 2		  //		     curva 3		   //			 recta 8	      //			  recta 9		   //
	vec3 RoadPosition[62] = { {390,2.5,-10}, {370,2.5,0}, {373,2.5,116}, {360,2.5,105}, {330,2.5,135}, {315,2.5,115}, {305,2.5,160}, {280,2.5,140}, {255,2.5,185}, {255,2.5,165}, {205,2.5,135}, {230,2.5,135}, {290,2.5,105}, {265,2.5,85}, {325,2.5,75}, {350,2.5,75}, {275,2.5,15}, {300,2.5,-5}, {340,2.5,-125}, {360,2.5,-150}, {280,2.5,-155}, {275,2.5,-180}, {250,2.5,-105}, {225,2.5,-105},};
	//                               //           recta 1        //             curva 1          //             recta 2          //			recta 3		    //			recta 4		        //			recta 5		     //			recta 6		       //			recta 7  	    //			 curva 2   		 //			  curva 3         //	       recta 8	          //	       recta 9	        //
	BodyRotation RoadRotation[62] = { {0, {0,1,0}}, {0, {0,1,0}}, {-45, {0,1,0}}, {-45, {0,1,0}}, {-90, {0,1,0}}, {-90, {0,1,0}}, {0, {0,1,0}}, {0, {0,1,0}}, {-90, {0,1,0}}, {-90, {0,1,0}}, {0, {0,1,0}}, {0, {0,1,0}}, {-90, {0,1,0}}, {-90, {0,1,0}}, {0, {0,1,0}}, {0, {0,1,0}}, {45, {0,1,0}}, {45, {0,1,0}}, {45, {0,1,0}}, {45, {0,1,0}}, {-90, {0,1,0}}, {-90, {0,1,0}}, {0, {0,1,0}}, {0, {0,1,0}} };
	for (int i = 0; i < num_roads; i++)
	{
		RoadCubes.PushBack(new Cube(RoadSize[i].sizeX, RoadSize[i].sizeY, RoadSize[i].sizeZ));
		RoadCubes[i]->SetPos(RoadPosition[i].x, RoadPosition[i].y, RoadPosition[i].z);
		RoadCubes[i]->SetRotation(RoadRotation[i].angle, RoadRotation[i].axis_pos);
		RoadCubes[i]->color = Yellow;
		bodyRoadCubes.PushBack(App->physics->AddBody(*(RoadCubes[i]), 0.0f));
	}
}