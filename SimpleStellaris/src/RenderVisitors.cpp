#include "SceneNodeVisitors.h"
#include "ECSGame.h"
#include "Components.h"
#include "CommonGameCode.h"
#include "EntitiesFunctions.h"
#include <random>
#include <iostream>
#include <SFML/Audio.hpp>
#include "SpaceObjectTypes.h"



//Render processing function
//Because I separated UI from Scene there is not need to check for UI part
void SceneNodeVisitorRender::ProcessNode(SceneNode& node)
{
    std::shared_ptr<Entity> spEntity = node.GetEntity().lock();
    //Get deltatime
    //float dt = ECSGame::Instance().GetDeltaTime();
    //Check if pointer is valid
    if (spEntity != nullptr)
    {
        //Draw entity if not hidden
        if (!spEntity->hidden)
        {
            //Now check which type of components entity has, because
            //different components will be displayed differently
            if (spEntity->HasComponent(ComponentType::RectangleShape))
            {
                //Get component
                std::shared_ptr<RectangleShapeComponent> spEntityRecShape = GetRectangleShapeComponent(*spEntity);
                //Get absolute position of the entity in the world
                sf::RenderStates states;
                states.transform = node.GetCombinedTransform();
                //Draw entity if not hidden
                renderWindow.draw(spEntityRecShape->shape, states);
                renderedEntities++;
            }
            else if (spEntity->HasComponent(ComponentType::TileMap))
            {
                //Get component
                std::shared_ptr<TileMapComponent> spEntityTileMap = GetTileMapComponent(*spEntity);

                renderedEntities++;
                spEntityTileMap->tileMap.Render(renderWindow, node.GetCombinedTransform());
            }
            else
                didNotRenderedEntities++;
        }
        else
            didNotRenderedEntities++;
    }
}



void SceneNodeVisitorRender::OutputRenderStatistics() 
{
    std::cout << "Entities rendered: "<<renderedEntities<<"; did not render: "<<didNotRenderedEntities<<'\n';
}



//Render UI elements
//Because I separated UI from Scene there is not need to check for UI part
void SceneNodeVisitorRenderUI::ProcessNode(SceneNode& node)
{
    std::shared_ptr<Entity> spEntity = node.GetEntity().lock();
    //Check if pointer is valid
    if (spEntity != nullptr)
    {
        //Draw entity if not hidden
        if (!spEntity->hidden)
        {
            //And now draw entities differently, depending on which other components they have
            if (spEntity->HasComponent(ComponentType::RectangleShape))
            {
                //Get component
                std::shared_ptr<RectangleShapeComponent> spEntityRecShape = GetRectangleShapeComponent(*spEntity);
                //Get absolute position of the entity in the world
                sf::RenderStates states;
                states.transform = node.GetCombinedTransform();
                //Draw entity
                renderWindow.draw(spEntityRecShape->shape, states);
                renderedEntities++;
            }
            else if (spEntity->HasComponent(ComponentType::Text))
            {
                //Get component
                std::shared_ptr<TextComponent> spEntityUI = GetTextComponent(*spEntity);
                //Get absolute position of the entity in the world
                sf::RenderStates states;
                states.transform = node.GetCombinedTransform();
                //Draw entity
                renderWindow.draw(*(spEntityUI->text), states);
                renderedEntities++;
            }
            else if (spEntity->HasComponent(ComponentType::OrbitVisualizer))
            {
                //Get component
                std::shared_ptr<OrbitVisualizerComponent> spEntityUI = GetOrbitVisualizerComponent(*spEntity);
                //Get absolute position of the entity in the world
                sf::RenderStates states;
                states.transform = node.GetCombinedTransform();
                //Draw entity
                renderWindow.draw(spEntityUI->orbitShape, states);
                renderedEntities++;
            }
            else
                didNotRenderedEntities++;
        }
        else
            didNotRenderedEntities++;
    }
}



void SceneNodeVisitorRenderUI::OutputRenderStatistics()
{
    std::cout << "Entities rendered: " << renderedEntities << "; did not render: " << didNotRenderedEntities << '\n';
}