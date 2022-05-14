#include "RoboCatServerPCH.hpp"


MouseServer::MouseServer()
{
}

void MouseServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject(this);
}


bool MouseServer::HandleCollisionWithCat(RoboCat* inCat)
{
	//kill yourself!
	SetDoesWantToDie(true);

	//+1 for corn
	ScoreBoardManager::sInstance->IncScore(inCat->GetPlayerId(), 1);
	return false;
}