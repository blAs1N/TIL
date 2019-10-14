#pragma once

#include "Actor.h"
#include "SoundEvent.h"
#include "Math.h"

class FPSActor : public Actor {
public:
	FPSActor(Game* inGame);

	void ActorInput(const InputState& inputState) override;
	void UpdateActor(float deltaTime) override;

	void SetFootstepSurface(float value);

private:
	class MeshComponent* mesh;
	class MoveComponent* move;
	class AudioComponent* audio;
	
	Vector2 speed;

	SoundEvent footstep;
	float lastFootstep;
};
