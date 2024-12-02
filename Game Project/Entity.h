#pragma once

#include "StepTimer.h"
#include "pch.h"
#include "DeviceResources.h"

using namespace DirectX;

class Entity
{
	public:
		Entity(std::vector<std::vector<std::string>> files,
			std::shared_ptr<DX::DeviceResources> m_deviceResources, 
			double animation_delay,
			double start_time,
			int x_loc,
			int y_loc,
			int width,
			int height);

		DirectX::SimpleMath::Vector2 get_loc();

		DirectX::SimpleMath::Vector2 get_origin();

		void set_loc(int x, int y);

		void update_loc();

		DirectX::SimpleMath::Vector2 get_speed();

		float get_scale();

		void update_scale();

		void update_speed(int x, int y);

		int get_facing();

		void update_facing(int dirr);

		int get_hp();

		void update_hp(int diff);

		bool is_interactable();

		void set_interactable(bool set);

		bool is_collidable();

		void set_collidable(bool set);

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> get_frame(double curr_time);

		void reset();

	private:
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		std::vector<std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>> textures;

		int current_frame;
		double animation_delay;
		double last_update;

		DirectX::SimpleMath::Vector2 loc;
		DirectX::SimpleMath::Vector2 speed;
		DirectX::SimpleMath::Vector2 size;
		int facing;

		int hp;
		int max_hp;

		bool interactable;
		bool collidable;
};

