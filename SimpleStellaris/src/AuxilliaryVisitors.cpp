#include "SceneNodeVisitors.h"
#include "ECSGame.h"
#include "Components.h"
#include "CommonGameCode.h"
#include "EntitiesFunctions.h"
#include <random>
#include <iostream>
#include <SFML/Audio.hpp>
#include "SpaceObjectTypes.h"

//Auxiliary visitors, are visitors which are not called by any system
//They are called by other visitors


//Render UI elements
void SceneNodeVisitorRenderUI::ProcessNode(SceneNode& node) 
{
    std::shared_ptr<Entity> spEntity = node.GetEntity().lock();
    //Check if pointer is valid
    if (spEntity != nullptr)
    {
        //Check if entity has UI component
        if (spEntity->HasComponent(ComponentType::UIPart) && spEntity->HasComponent(ComponentType::Text))
        {
            if (spEntity->HasComponent(ComponentType::Text))
            {
                //Get component
                std::shared_ptr<TextComponent> spEntityUI = GetTextComponent(*spEntity);

                //Get absolute position of the entity in the world
                sf::RenderStates states;
                states.transform = node.GetCombinedTransform();
                //Draw entity
                renderWindow.draw(*(spEntityUI->text), states);
            }
        }
    }
}