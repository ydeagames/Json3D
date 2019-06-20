#pragma once
#include "Transform.h"
#include <Utilities/TypeId.h>

class GameContext;
class Component;

// �Q�[���I�u�W�F�N�g
class GameObject
{
private:
	std::unordered_map<type_id_t, std::shared_ptr<Component>> m_components;

public:
	std::shared_ptr<Transform> transform;

private:
	template<typename T>
	struct Impl
	{
		template<typename U = T>
		static auto AddComponent(std::unordered_map<type_id_t, std::shared_ptr<Component>>& components, const std::shared_ptr<U>& component)
			-> typename std::enable_if<!std::is_same<U, Component>::value>::type
		{
			components.insert(std::make_pair(type_id<U>(), component));
			using t = U::Base;
			AddComponent<t>(components, component);
		}

		template<typename U = T>
		static auto AddComponent(std::unordered_map<type_id_t, std::shared_ptr<Component>>& components, const std::shared_ptr<U>& component)
			-> typename std::enable_if<std::is_same<U, Component>::value>::type
		{
			components.insert(std::make_pair(type_id<U>(), component));
		}
	};

public:
	template<typename T, typename... Args>
	std::shared_ptr<T> AddComponent(Args&&... args)
	{
		auto component = std::make_shared<T>(std::forward<Args>(args)...);
		component->gameObject = this;
		Impl<T>::AddComponent(m_components, component);
		return component;
	}

	template<typename T>
	std::shared_ptr<T> GetComponent()
	{
		return m_components.at(type_id<T>());
	}

public:
	GameObject()
	{
		transform = AddComponent<Transform>();
	}

	virtual ~GameObject() = default;

public:
	// ����
	virtual void Initialize(GameContext& context)
	{
		for (auto& component : m_components)
			component.second->Initialize(context);
	}

	// �X�V
	virtual void Update(GameContext& context)
	{
		for (auto& component : m_components)
			component.second->Update(context);
	}

	// �`��
	virtual void Render(GameContext& context)
	{
		for (auto& component : m_components)
			component.second->Render(context);
	}

	// �j��
	virtual void Finalize(GameContext& context)
	{
		for (auto& component : m_components)
			component.second->Finalize(context);
	}
};

