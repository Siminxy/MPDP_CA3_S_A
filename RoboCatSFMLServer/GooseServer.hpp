class GooseServer : public Goose
{
public:
	static GameObjectPtr	StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn(new GooseServer()); }
	void HandleDying() override;

	virtual bool		HandleCollisionWithCat(RoboCat* inCat) override;

	virtual void Update() override;

protected:
	GooseServer();

private:
	float mTimeToDie;

};

