#include "RoboCatShared.hpp"

#include <windows.h>
#include <chrono>

#include "SFML\Graphics.hpp"
#include "SFML/Network/IpAddress.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <SFML/Network/Packet.hpp>

#include "InputManager.hpp"
#include "SpriteComponent.hpp"
#include "PlayerSpriteComponent.hpp"
#include "TextureManager.hpp"
#include "FontManager.hpp"
#include "RenderManager.hpp"
#include "WindowManager.hpp"

#include "RoboCatClient.hpp"
#include "MouseClient.hpp"
#include "YarnClient.hpp"

#include "HUD.hpp"


#include "ReplicationManagerClient.hpp"
#include "NetworkManagerClient.hpp"
#include "Client.hpp"


