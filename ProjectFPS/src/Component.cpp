//component class so that components like transform and mesh renderer can be added
#include "pch.h"
#include "Component.h"

void Component::SetGameObject(GameObject& gameObject)
{
	this->gameObject = &gameObject;
}