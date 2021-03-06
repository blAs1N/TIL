#include "FPSActor.h"
#include "Game.h"
#include "Renderer.h"
#include "AudioSystem.h"
#include "InputSystem.h"
#include "FPSCamera.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "AudioComponent.h"

FPSActor::FPSActor(Game* inGame)
	: Actor(inGame),
	camera(new FPSCamera(this)),
	mesh(nullptr),
	move(new MoveComponent(this)),
	audio(new AudioComponent(this)),
	speed(),
	fpsModel(new Actor(inGame)),
	startSphere(new Actor(inGame)),
	endSphere(new Actor(inGame)),
	footstep(),
	lastFootstep(0.0f) {

	fpsModel->SetScale(0.75f);
	mesh = new MeshComponent(fpsModel);
	mesh->SetMesh(inGame->GetRenderer()->GetMesh("Assets/Rifle.gpmesh"));

	const Vector3 SpherePos{ 10000.0f, 0.0f, 0.0f };

	startSphere->SetPosition(SpherePos);
	startSphere->SetScale(0.25f);

	auto mc = new MeshComponent(startSphere);
	mc->SetMesh(inGame->GetRenderer()->GetMesh("Assets/Sphere.gpmesh"));

	endSphere->SetPosition(SpherePos);
	endSphere->SetScale(0.25f);

	mc = new MeshComponent(endSphere);
	mc->SetMesh(inGame->GetRenderer()->GetMesh("Assets/Sphere.gpmesh"));

	footstep = audio->PlayEvent("event:/Footstep");
	footstep.SetPaused(true);
}

void FPSActor::ActorInput(const InputState& inputState) {
	Actor::ActorInput(inputState);

	const auto useController = inputState.controllers.size() > 0;
	speed = Vector2::Zero;

	if (useController) {
		speed = inputState.controllers[0].GetLeftStick();
		speed *= 400.0f;

		const auto tmp = speed.x;
		speed.x = speed.y;
		speed.y = tmp;
	}
	else {
		if (inputState.keyboard.GetKeyValue(SDL_SCANCODE_W))
			speed.x += 400.0f;
		if (inputState.keyboard.GetKeyValue(SDL_SCANCODE_S))
			speed.x -= 400.0f;
		if (inputState.keyboard.GetKeyValue(SDL_SCANCODE_A))
			speed.y -= 400.0f;
		if (inputState.keyboard.GetKeyValue(SDL_SCANCODE_D))
			speed.y += 400.0f;
	}

	move->SetForwardSpeed(speed.x);
	move->SetStrafeSpeed(speed.y);

	const Vector2 MaxAngularSpeed{ Math::Pi * 8, Math::Pi * 8 };

	auto angularSpeed = MaxAngularSpeed;

	if (useController) {
		angularSpeed *= inputState.controllers[0].GetRightStick() * 0.25f;
		angularSpeed.y *= -1.0f;
	}
	else
		angularSpeed *= inputState.mouse.GetPosition() / 500.0f;

	move->SetAngularSpeed(angularSpeed.x);
	camera->SetPitchSpeed(angularSpeed.y);

	const auto SetSphere = [this, renderer = GetGame()->GetRenderer()]{
		startSphere->SetPosition(renderer->Unproject(Vector3::Zero));
		endSphere->SetPosition(renderer->Unproject(Vector3{ 0.0f, 0.0f, 0.9f }));
	};

	if (inputState.controllers.size() > 0) {
		if (inputState.controllers[0].GetButtonState(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == ButtonState::EPressed)
			SetSphere();
	}
	else {
		if (inputState.mouse.GetButtonState(SDL_BUTTON_LEFT) == ButtonState::EPressed)
			SetSphere();
	}
}

void FPSActor::UpdateActor(const float deltaTime) {
	Actor::UpdateActor(deltaTime);

	lastFootstep -= deltaTime;
	if (speed.LengthSquared() > 0.0f && lastFootstep <= 0.0f) {
		footstep.SetPaused(false);
		footstep.Restart();
		lastFootstep = 0.5f;
	}

	const Vector3 ModelOffset{ 10.0f, 10.0f, -10.0f };
	auto modelPos = GetPosition();
	modelPos += GetForward() * ModelOffset.x;
	modelPos += GetRight() * ModelOffset.y;
	modelPos.z += ModelOffset.z;
	fpsModel->SetPosition(modelPos);

	const auto rot = Quaternion::Concatenate(GetRotation(),
		Quaternion{ GetRight(), camera->GetPitch() });
	fpsModel->SetRotation(rot);
}

void FPSActor::SetFootstepSurface(const float value) {
	footstep.SetPaused(true);
	footstep.SetParameter("Surface", value);
}

void FPSActor::SetVisible(const bool visible) {
	mesh->SetVisible(visible);
}