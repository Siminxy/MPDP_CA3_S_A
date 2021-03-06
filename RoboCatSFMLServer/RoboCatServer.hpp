enum ECatControlType
{
	ESCT_Human,
	ESCT_AI
};

class RoboCatServer : public RoboCat
{
public:
	static GameObjectPtr	StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn(new RoboCatServer()); }
	virtual void HandleDying() override;

	virtual void Update() override;
	bool HandleCollisionWithCat(RoboCat* inCat) override;

	void SetCatControlType(ECatControlType inCatControlType) { mCatControlType = inCatControlType; }

	void TakeDamage(int inDamagingPlayerId);

protected:
	RoboCatServer();

private:

	ECatControlType	mCatControlType;

	int			mHitCooldown;
	bool		mCanBeHit;

};

