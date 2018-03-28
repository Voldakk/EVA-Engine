#include "GameObject.hpp"

#include "Scene.hpp"

namespace EVA
{

    GameObject::GameObject(Scene *scene)
    {
        m_Scene = scene;
        m_Transform = std::make_shared<Transform>(this);
    }

    void GameObject::Update(const float deltaTime)
    {
        for (auto &component : m_Components)
        {
            component->Update(deltaTime);
        }
    }

    void GameObject::SetParent(GameObject *newParent)
    {
        m_Transform->SetParent(newParent->m_Transform);
    }

    void GameObject::SetParent(std::shared_ptr<GameObject> &newParent)
    {
        m_Transform->SetParent(newParent->m_Transform);
    }


    int GameObject::GetChildIndex(Transform *child) const
    {
        return m_Transform->GetChildIndex(child);
    }

    int GameObject::GetChildIndex(GameObject *child) const
    {
        return m_Transform->GetChildIndex(child);
    }

}