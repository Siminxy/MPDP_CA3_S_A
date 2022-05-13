#include "RoboCatServerPCH.hpp"

RoboCatServer::RoboCatServer() :
	mCatControlType(ESCT_Human)
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

void RoboCatServer::TakeDamage(int inDamagingPlayerId)
{
	float time = Timing::sInstance.GetFrameStartTime();

	if (Timing::sInstance.GetFrameStartTime() > mTimeBetweenHits)
	{
		mHealth--;

		//not exact, but okay
		mTimeBetweenHits = time + mTimeBetweenHits;

		//score one for damaging player...
		ScoreBoardManager::sInstance->IncScore(inDamagingPlayerId, 1);
		this->DecScale(0.25f);

		//If no health or too small, they are considered dead
		if (mHealth <= 0.f || this->GetScale() <= 0.25f)
		{
			//score one for damaging player...
			ScoreBoardManager::sInstance->IncScore(inDamagingPlayerId, 5);

			//and you want to die
			SetDoesWantToDie(true);

			//tell the client proxy to make you a new cat
			ClientProxyPtr clientProxy = NetworkManagerServer::sInstance->GetClientProxy(GetPlayerId());
			if (clientProxy)
			{
				clientProxy->HandleCatDied();
			}
		}

		//tell the world our health dropped
		NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), ECRS_Health);
	}

	
}

