#pragma once
#include <map>
#include <memory>
using std::unique_ptr;



template<typename ResourceID, typename Resource>
class ResourceManager {
public:
    ResourceManager() = default;

    void load(ResourceID resource, const std::string& filename) {
        if(resources.find(resource) != resources.end()) {
            throw std::logic_error("Resource already loaded: " + filename);
        }
        resources[resource].reset(new Resource());
        if(not resources.at(resource)->loadFromFile(filename)) {
            throw std::runtime_error("Error loading resource from " + filename);
        }
    }

    Resource& get(ResourceID resource) {
        try {
            return *resources.at(resource);

        } catch (std::out_of_range) {
            throw std::logic_error("Resource not found");
        }
    }

private:
    std::map<ResourceID, unique_ptr<Resource>> resources;
};
