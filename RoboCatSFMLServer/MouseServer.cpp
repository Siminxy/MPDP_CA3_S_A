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

	ScoreBoardManager::sInstance->IncScore(inCat->GetPlayerId(), 1);

	Server::MoreCorn(1);

	return false;
}