#include "RoboCatClientPCH.hpp"

bool Client::StaticInit()
{
	// Create the Client pointer first because it initializes SDL
	Client* client = new Client();
	InputManager::StaticInit();

	WindowManager::StaticInit();
	FontManager::StaticInit();
	TextureManager::StaticInit();
	RenderManager::StaticInit();
	

	HUD::StaticInit();

	s_instance.reset(client);

	return true;
}

sf::String GetAddressFromFile()
{
	{
		//Try to open existing file ip.txt
		std::ifstream input_file("clientip.txt");
		std::string ip_address;
		if (input_file >> ip_address)
		{
			return ip_address;
		}
	}

	//If open/read failed, create a new file
	std::ofstream output_file("clientip.txt");
	std::string local_address = "127.0.0.1:50000 name";
	output_file << local_address;
	return local_address;
}

Client::Client()
{
	GameObjectRegistry::sInstance->RegisterCreationFunction('RCAT', RoboCatClient::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('MOUS', MouseClient::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('YARN', YarnClient::StaticCreate);

	string destination;// = StringUtils::GetCommandLineArg(1);
	string name;// = StringUtils::GetCommandLineArg(2);

	string ipname = GetAddressFromFile();
	
	//vector<string> vectorin;

	for(int i = 0; i >= 1; i++)
	{
		
		//vector<string> in;
		std::istringstream ss(ipname);
		string thing;
		
		while(std::getline(ss, thing, ' '))
		{
			//vectorin.push_back(thing);
			if(i==0)
			{
				destination = thing;
			}
			else
			{
				name = thing;
			}
		}
	}

	//destination = vectorin.at(0);
	//name = vectorin.at(1);

	SocketAddressPtr serverAddress = SocketAddressFactory::CreateIPv4FromString(destination);


	NetworkManagerClient::StaticInit(*serverAddress, name);

	//NetworkManagerClient::sInstance->SetSimulatedLatency(0.0f);
}



void Client::DoFrame()
{
	InputManager::sInstance->Update();

	Engine::DoFrame();

	NetworkManagerClient::sInstance->ProcessIncomingPackets();

	RenderManager::sInstance->Render();

	NetworkManagerClient::sInstance->SendOutgoingPackets();
}

void Client::HandleEvent(sf::Event& p_event)
{
	switch (p_event.type)
	{
	case sf::Event::KeyPressed:
		InputManager::sInstance->HandleInput(EIA_Pressed, p_event.key.code);
		break;
	case sf::Event::KeyReleased:
		InputManager::sInstance->HandleInput(EIA_Released, p_event.key.code);
		break;
	default:
		break;
	}
}

bool Client::PollEvent(sf::Event& p_event)
{
	return WindowManager::sInstance->pollEvent(p_event);
}


