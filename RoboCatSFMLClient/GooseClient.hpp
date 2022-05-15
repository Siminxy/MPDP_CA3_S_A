class GooseClient : public Goose
{
public:
	static	GameObjectPtr	StaticCreate() { return GameObjectPtr(new GooseClient()); }

protected:
	GooseClient();

private:

	SpriteComponentPtr	mSpriteComponent;
};
