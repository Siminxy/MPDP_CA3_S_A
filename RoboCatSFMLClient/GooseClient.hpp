class GooseClient : public Goose
{
public:
	static	GameObjectPtr	StaticCreate() { return GameObjectPtr(new GooseClient()); }

	virtual void		Read(InputMemoryBitStream& inInputStream) override;

protected:
	GooseClient();

private:

	SpriteComponentPtr	mSpriteComponent;
};
