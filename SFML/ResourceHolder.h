#pragma once
#ifndef RESOURCE_HOLDER
#define RESOURCE_HOLDER

#include<map>
#include<string>
#include<memory>
#include<stdexcept>
#include<iostream>
#include<cassert>
template<typename Resource, typename Identifier>
class ResourceHolder
{
public:
	void load(Identifier id, const std::string& filename);
	void clear();
	template<typename Parameter>
	void load(Identifier id, const std::string& filename, const Parameter& secondParam);
	Resource& get(Identifier id);
	const Resource& get(Identifier id) const;
private:
	void insertResource(Identifier id, std::unique_ptr<Resource> resource);
	
private:
	std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};
#include"ResourceHolder.inl"
#endif // !RESOURCE_HOLDER