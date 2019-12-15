#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	//// Car properties ----------------------------------------
	
	car.chassis_size.Set(2, 0.8, 6);
	car.chassis_offset.Set(0, 1.45, 0);

	car.upper_chassis.Set(2, 0.8, 4);
	car.upper_chassis_offset.Set(0, 2.2, -0.2);

	car.upper_chassis2.Set(1.7, 0.6, 0.1);
	car.upper_chassis2_offset.Set(0, 2.2, -2.2);

	car.top_chassis.Set(2.1, 0.5, 2.7);
	car.top_chassis_offset.Set(0, 2.2, -0.2);

	car.mass = 500.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 5.0f;
	car.suspensionDamping = 10.0f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 2000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(INITIAL_POS);

	initialPosMatrix = mat4x4
	(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		380, 2, -10, 0);

	vehicle->SetTransform(initialPosMatrix.M);

	message = "BEAT THE TIME TRIAL (1:30)";

	timer.Start();

	laps = 0;
	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
	{
		App->audio->PlayFx(App->audio->accelerateFx);
	}
	
	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		acceleration = MAX_ACCELERATION;
	}

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if(turn < TURN_DEGREES)
			turn +=  TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if(turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		if (vehicle->GetKmh() > 0.0f) 
			brake = BRAKE_POWER;
		if (vehicle->GetKmh() <= 0.0f) 
			acceleration = -(MAX_ACCELERATION / 2);
	}

	CarPosX = vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getX();
	CarPosZ = vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getZ();
	
	if (CarPosX > 370 && CarPosX < 390 && CarPosZ > 10 && CarPosZ < 11)
	{
		if (lapCounted == false) 
		{
			if (laps < 3)
			{
				laps++;
				lapCounted = true;
				LapTimer.Start();
			}
			else if (laps == 3)
			{
				timer.Stop();

				if (timer.Read() <= 90000)
				{
					win = true;
				}

				else if (timer.Read() > 90000)
				{
					lose = true;
				}
			}
		}
	}

	if (LapTimer.Read() > 10000) 
	{
		lapCounted = false;
	}

	//win condition
	if (win == true && lose == false)
	{
		App->camera->ChangeCamera(true, false);
		App->audio->PlayFx(App->audio->winFx, 1);
		message = "GREAT JOB";
		timer.Stop();
	}

	//lose condition
	if (lose == true && win == false)
	{
		App->camera->ChangeCamera(true, false);
		App->audio->PlayFx(App->audio->loseFx, 1);
		message = "YOU FAILED";
		timer.Stop();
	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();
	CameraToPlayer();

	int lap_sec = timer.Read() / 1000;
	int lap_min = lap_sec / 60;
	lap_sec -= lap_min * 60;

	char title[200];
	sprintf_s(title, "%s || Laps: %d || Time: %.2i:%.2i",  message, laps, lap_min, lap_sec);
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}

void ModulePlayer::CameraToPlayer()
{
	newCarPos = vehicle->vehicle->getChassisWorldTransform();

	initialCarPos =
	{
		newCarPos.getOrigin().getX(),
		newCarPos.getOrigin().getY(),
		newCarPos.getOrigin().getZ()
	};

	newCarDirection =
	{
		newCarPos.getBasis().getColumn(2).getX(),
		newCarPos.getBasis().getColumn(2).getY(),
		newCarPos.getBasis().getColumn(2).getZ()
	};

	App->camera->Position.x = newCameraPos.x;
	App->camera->Position.y = newCameraPos.y;
	App->camera->Position.z = newCameraPos.z;

	newCameraPos = initialCarPos - 9 * newCarDirection;
	App->camera->Position.y = initialCarPos.y + 6;

}

void ModulePlayer::MySetPos(vec3 newPos)
{
	vehicle->SetPos(newPos.x, newPos.y, newPos.z);
}

void ModulePlayer::Respawn(vec3 respawn_pos)
{
	MySetPos(respawn_pos);
	vehicle->vehicle->getRigidBody()->setLinearVelocity({ 0,0,0 });
	vehicle->vehicle->getRigidBody()->setAngularVelocity({ 0, 0, 0 });

}