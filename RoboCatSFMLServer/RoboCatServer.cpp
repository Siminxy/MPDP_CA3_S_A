#include "RoboCatServerPCH.hpp"

RoboCatServer::RoboCatServer() :
	mCatControlType(ESCT_Human),
	mHitCooldown(5),
	mCanBeHit(true)
{}

void RoboCatServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject(this);
}

void RoboCatServer::Update()
{
	RoboCat::Update();

	Vector3 oldLocation = GetLocation();
	Vector3 oldVelocity = GetVelocity();
	float oldRotation = GetRotation();

	//are you controlled by a player?
	//if so, is there a move we haven't processed yet?
	if (mCatControlType == ESCT_Human)
	{
		ClientProxyPtr client = NetworkManagerServer::sInstance->GetClientProxy(GetPlayerId());
		if (client)
		{
			MoveList& moveList = client->GetUnprocessedMoveList();
			for (const Move& unprocessedMove : moveList)
			{
				const InputState& currentState = unprocessedMove.GetInputState();

				float deltaTime = unprocessedMove.GetDeltaTime();

				ProcessInput(deltaTime, currentState);
				SimulateMovement(deltaTime);

				//LOG( "Server Move Time: %3.4f deltaTime: %3.4f left rot at %3.4f", unprocessedMove.GetTimestamp(), deltaTime, GetRotation() );

			}

			moveList.Clear();
		}
	}
	else
	{
		//do some AI stuff
		SimulateMovement(Timing::sInstance.GetDeltaTime());
	}


	if (!RoboMath::Is2DVectorEqual(oldLocation, GetLocation()) ||
		!RoboMath::Is2DVectorEqual(oldVelocity, GetVelocity()) ||
		oldRotation != GetRotation())
	{
		NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), ECRS_Pose);
	}

}

bool RoboCatServer::HandleCollisionWithCat(RoboCat* inCat)
{
	if ((int)Timing::sInstance.GetFrameStartTime() % mHitCooldown + 1 == mHitCooldown)
	{
		mCanBeHit = true;
	}

	//If player can be hit && both players scores are over 20
	if (mCanBeHit && ScoreBoardManager::sInstance->CanAttackOthers(GetPlayerId()) && ScoreBoardManager::sInstance->CanAttackOthers(inCat->GetPlayerId()))
	{
		if (inCat->GetPlayerId() != GetPlayerId())
		{
			if (ScoreBoardManager::sInstance->GetPlayerScore(inCat->GetPlayerId()) < ScoreBoardManager::sInstance->GetPlayerScore(GetPlayerId()))
			{
				static_cast<RoboCatServer*>(inCat)->TakeDamage(GetPlayerId());
				inCat->IncScale(0.25);
				DecScale(0.25f);
			}
			else if (ScoreBoardManager::sInstance->GetPlayerScore(inCat->GetPlayerId()) > ScoreBoardManager::sInstance->GetPlayerScore(GetPlayerId()))
			{
				static_cast<RoboCatServer*>(GetAsCat())->TakeDamage(GetPlayerId());
				IncScale(0.25);
				inCat->DecScale(0.25f);
			}

			
		}
	}

	//Inform of scale
	NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), ECRS_Health);

	return false;
}

void RoboCatServer::TakeDamage(int inDamagingPlayerId)
{
	mHealth--;

	//If no health they are considered dead
	if (mHealth <= 0.f)
	{
		//+10 for kills
		ScoreBoardManager::sInstance->IncScore(inDamagingPlayerId, 10);

		//and you want to die
		SetDoesWantToDie(true);

		//tell the client proxy to make you a new cat
		ClientProxyPtr clientProxy = NetworkManagerServer::sInstance->GetClientProxy(GetPlayerId());
		if (clientProxy)
		{
			clientProxy->HandleCatDied();
		}
	}
	else
	{
		//+2 for hits
		ScoreBoardManager::sInstance->IncScore(inDamagingPlayerId, 2);

		//-2 points for player
		ScoreBoardManager::sInstance->DecScore(GetPlayerId(), 2);
	}

	//tell the world our health dropped
	NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), ECRS_Health);
}

