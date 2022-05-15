#include "RoboCatServerPCH.hpp"


GooseServer::GooseServer()
{
	//yarn lives a second...
	mTimeToDie = Timing::sInstance.GetFrameStartTime() + 1.f;
}

void GooseServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject(this);
}


void GooseServer::Update()
{
	Goose::Update();

	if (Timing::sInstance.GetFrameStartTime() > mTimeToDie)
	{
		SetDoesWantToDie(true);
	}

}

bool GooseServer::HandleCollisionWithCat(RoboCat* inCat)
{
	if (inCat->GetPlayerId() != GetPlayerId())
	{
		//kill yourself!
		SetDoesWantToDie(true);

		static_cast<RoboCatServer*>(inCat)->TakeDamage(GetPlayerId());

	}

	return false;
}



