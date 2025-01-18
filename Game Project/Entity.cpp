#include "pch.h"
#include "Entity.h"



using Microsoft::WRL::ComPtr;

Entity::Entity(std::vector<std::vector<std::string>> files,
	std::shared_ptr<DX::DeviceResources> m_deviceResources,
	double animation_delay,
	double start_time,
	int x_loc,
	int y_loc,
	int width,
	int height)
{
	auto device = m_deviceResources->GetD3DDevice();

	if (files.empty()) {
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
		DX::ThrowIfFailed(
			CreateWICTextureFromFile(device, L"cat.png",
				nullptr,
				texture.ReleaseAndGetAddressOf()));
		std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> v1;
		v1.emplace_back(texture);
		textures.emplace_back(v1);
	}
	else {
		for (int i = 0; i < files.size(); i++)
		{
			std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> v1;
			for (int j = 0; j < files.at(i).size(); j++)
			{
				Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
				DX::ThrowIfFailed(
					CreateWICTextureFromFile(device, std::wstring(files.at(i).at(j).begin(), files.at(i).at(j).end()).c_str(),
						nullptr,
						texture.ReleaseAndGetAddressOf()));
				
				v1.emplace_back(texture);
			}
			textures.emplace_back(v1);
		}
	}

	this->current_frame = 0;
	this->animation_delay = animation_delay;
	this->last_update = start_time;

	this->loc.x = x_loc, this->loc.y = y_loc;
	this->size.x = width, this->size.y = height;
	this->facing = 0;

	this->m_deviceResources = m_deviceResources;
}


DirectX::SimpleMath::Vector2 Entity::get_loc()
{
	DirectX::SimpleMath::Vector2 screenLoc(loc.x, loc.y);
	return screenLoc;
}

DirectX::SimpleMath::Vector2 Entity::get_origin()
{
	return size;
}

void Entity::set_loc(int x, int y) 
{
	loc.x = x, loc.y = y;
}

void Entity::update_loc() 
{
	loc.x += speed.x, loc.y += speed.y;
}

DirectX::SimpleMath::Vector2 Entity::get_speed()
{
	return speed;
}

void Entity::update_speed(int x, int y)
{
	speed.x += x, speed.y += y;
}

int Entity::get_facing()
{
	return facing;
}

float Entity::get_scale()
{
	return 1.0f;
}

void Entity::update_scale() {

}

void Entity::update_facing(int dirr) 
{
	facing = dirr;
}

int Entity::get_hp() 
{
	return hp;
}

void Entity::update_hp(int diff)
{
	hp += diff;
	if (hp > max_hp)
	{
		hp = max_hp;
	}
}

bool Entity::is_interactable()
{
	return interactable;
}

void Entity::set_interactable(bool set)
{
	interactable = set;
}

bool Entity::is_collidable()
{
	return collidable;
}

void Entity::set_collidable(bool set) 
{
	collidable = set;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Entity::get_frame(double curr_time)
{
	if (curr_time > last_update + animation_delay)
	{
		last_update = curr_time;
		if (current_frame < textures.at(facing).size() - 1)
		{
			current_frame += 1;
		}
		else {
			current_frame = 0;
		}
	}
	return textures.at(facing).at(current_frame);
}

void Entity::reset()
{
	for (int i = 0; i < textures.size(); i++)
	{
		for (int j = 0; j < textures.at(i).size(); j++)
		{
			textures.at(i).at(j).Reset();
		}
	}
}