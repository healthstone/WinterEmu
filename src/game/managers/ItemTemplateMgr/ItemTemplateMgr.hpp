#pragma once

#include <memory>
#include <unordered_map>
#include "src/game/Entity/Item/ItemTemplate/ItemTemplate.hpp"

class BaseServer;

typedef std::unordered_map<uint32_t, ItemTemplate> ItemTemplateContainer;

class ItemTemplateMgr {
public:
    explicit ItemTemplateMgr(std::shared_ptr<BaseServer> server) : server_(std::move(server)) {}

    ~ItemTemplateMgr();

    void cleanUpBeforeDelete();

    void loadFromDB();

    ItemTemplate const* getItemTemplate(uint32_t entry) {
        auto itr = itemTemplateStore_.find(entry);
        if (itr != itemTemplateStore_.end())
            return &itr->second;
        return nullptr;
    }
    ItemTemplateContainer const& getItemTemplateStore() const { return itemTemplateStore_; }

private:
    std::weak_ptr<BaseServer> server_;
    ItemTemplateContainer itemTemplateStore_;
};