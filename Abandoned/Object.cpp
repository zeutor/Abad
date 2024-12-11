#include "Object.hpp"

std::unordered_multiset<Object*> Object::_allObjects;
int Object::ObjCount;

std::unordered_multiset<Object*>& Object::getAllObjects()
{
    return _allObjects;
}


//Выгрузка массива Object из файлика =)
void Object::Load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << " не удалось открыть файл: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) 
    {
        std::istringstream iss(line);

        int itemTypeInt, itemID, masterID;
        unsigned int cost;
        float posX, posY, sizeX, sizeY;
        bool isInInventory;

        std::string spritePath, effectIDsStr;

        if (!(iss >> itemTypeInt >> posX >> posY  >> isInInventory >> itemID >> spritePath >> effectIDsStr >> masterID >> cost)) {
            std::cerr << "Ошибка разборкки строчки: " << line << std::endl;
            continue;
        }

        ItemType itemType = static_cast<ItemType>(itemTypeInt);
        sf::Vector2f pos(posX * PIXELS_PER_CELL, posY * PIXELS_PER_CELL);


        sf::Texture* texture = new sf::Texture();
        if (!texture->loadFromFile(spritePath)) {
            std::cerr << "Не удалось загрузить текстуру: " << spritePath << std::endl;
            continue;
        }
  
        sf::Sprite sprite;
        sprite.setTexture(*texture);
        sprite.setScale(DEFAULT_OBJECT_SIZE, DEFAULT_OBJECT_SIZE);

        std::unordered_set<int> effectIDs;
        std::istringstream effectStream(effectIDsStr);
        std::string effect;
        while (std::getline(effectStream, effect, ',')) {
            effectIDs.insert(std::stoi(effect));
        }

        sprite.setPosition(pos);
        Object* newObj = new Object(itemType, pos, sprite, isInInventory, itemID, effectIDs, cost);
        newObj->setMasterID(masterID);
   
        _allObjects.insert(newObj);
    }

    file.close();
}

Object::Object(ItemType type, const sf::Vector2f& pos, const sf::Sprite& sprite, bool IsInInv, int itemID, std::unordered_set<int> effectIds, unsigned int cost)
    : _itemType(type), _isInInventory(IsInInv), _position(pos), _sprite(sprite), _itemID(itemID), _effectIDs(effectIds), _cost(cost) {
    ++ObjCount;
    uniqueId = ObjCount;
}


std::string Object::getEffectIDsAsString()  {
    std::ostringstream oss;

    for (const auto& id : _effectIDs) 
    {
        oss << id << ",";
    }

    std::string result = oss.str();
     
    if (!result.empty()) 
    {
        result.pop_back(); 
    }
    return result;
}

void Object::Sell(Character& Entitiy)
{
    UIManager* _manager = UIManager::getController();
    //Проверка находится предмет у вас или в сундуке
    if (this->getMasterID() != _manager->getSellerId()) {
        //Изменение ID владельца для предмета
        //Удаление предмета из инвентаря персонажа
        Entitiy.RemoveFromInventory(this->getUniqueId());
        //Обход всех MAP объектов и поиск с соотв. COmmutatorID (с которым мы обмениваемся предметами)
        _manager->insertItemToPlayerTemp(this->getUniqueId());
        _manager->addMoney_TempPlayer(this->getCost());
    }
    else if (_manager->findInPlayerInventory(this->getUniqueId()))
    {
        _manager->loseMoney_TempPlayer(this->getCost());
        _manager->DeleteteItemToPlayerTemp(this->getUniqueId());
        Entitiy.insertInInventory(this->getUniqueId());

    }
    else if (_manager->findInMerchantInventory(this->getUniqueId()))
    {
        _manager->loseMoney_TempMerchant(this->getCost());
        _manager->DeleteteItemInMerchantTemp(this->getUniqueId());
        std::unordered_map<unsigned int, Character*> allChar = Character::getAllChar();
        auto it = allChar.find(_manager->getSellerId());
        Character* seller = it->second;
        seller->insertInInventory(this->getUniqueId());
    }
    else
    {
        std::unordered_map<unsigned int, Character*> allChar = Character::getAllChar();
        auto it = allChar.find(_manager->getSellerId());
        Character* seller = it->second;
        seller->RemoveFromInventory(this->getUniqueId());
        _manager->insertItemToMechantTemp(this->getUniqueId());
        _manager->addMoney_TempMerchant(this->getCost());
    }

}

void Object::AcceptTrade(Character& Entitiy)
{
    UIManager* _manager = UIManager::getController();
    int SellerId = _manager->getSellerId();
    std::unordered_map<unsigned int, Character*> AllChar = Character::getAllChar();
    auto it = AllChar.find(SellerId);
    Character* seller = it->second;
    seller->addMoney(_manager->getTempMerchantMoney());
    Entitiy.addMoney(_manager->getTempPlayerMoney());

    seller->loseMoney(_manager->getTempPlayerMoney());
    Entitiy.loseMoney(_manager->getTempMerchantMoney());

    _manager->loseMoney_TempMerchant(_manager->getTempMerchantMoney());
    _manager->loseMoney_TempPlayer(_manager->getTempPlayerMoney());
    std::multiset<int> player_temp_inv = _manager->GetplayerTempInventory();
    std::multiset<int> merchant_temp_inv = _manager->GetmerchantTempInventory();

    std::unordered_multiset<Object*> allObj = Object::getAllObjects();
    for (auto item : player_temp_inv)
    {
        for (auto obj : allObj)
        {
            if (obj->getUniqueId() == item)
            {
                obj->setMasterID(seller->getID());
            }
        }
        seller->insertInInventory(item);
    }
    for (auto item1 : merchant_temp_inv)
    {
        for (auto obj : allObj)
        {
            if (obj->getUniqueId() == item1)
            {
                obj->setMasterID(1);
            }
        }
        Entitiy.insertInInventory(item1);
    }

}

void Object::Use(Character& Entity) {
    

}

void Object::Give(Character& Entity)
{
    //В дальнейшем будет реализация использования всех объектов, но сейчас реализована только 
    // передача вещей по нажатию ЛКМ во время окна с PICKUP MENU
    UIManager* _manager = UIManager::getController();
        //Проверка находится предмет у вас или в сундуке
        if (this->getMasterID() != _manager->getCommutatorID()) {
            //Изменение ID владельца для предмета
            this->setMasterID(_manager->getCommutatorID());
            //Удаление предмета из инвентаря персонажа
            Entity.RemoveFromInventory(this->getUniqueId());
            std::unordered_set<MapObject*> allMapObj = MapObject::getAllMapObjects();
            //Обход всех MAP объектов и поиск с соотв. COmmutatorID (с которым мы обмениваемся предметами)
            for (auto const& mapobj : allMapObj)
            {
                if (mapobj->getMasterID() == _manager->getCommutatorID())
                {
                    //если найден, то кастуем mapobj к Storage и используем методы чтобы вставить предмет в его "инвентарь"
                    Storage* chest_stor = dynamic_cast<Storage*>(mapobj);
                    chest_stor->insertItem(this->getUniqueId());
                    //Функция для отладки, фактически в консоль выводит все предметы Storage
                    chest_stor->printAllItems();
                }

            }
        }
        else
            //В том случае если предмет у сундука в инвентаре
        {
            std::unordered_set<MapObject*> allMapObj = MapObject::getAllMapObjects();
            for (auto const& mapobj : allMapObj)
            {
                //Находим этот сундук, кастуем к Storage и удаляем item
                if (mapobj->getMasterID() == _manager->getCommutatorID()) {
                    Storage* chest_stor = dynamic_cast<Storage*>(mapobj);
                    chest_stor->DeleteItem(this->getUniqueId());
                    chest_stor->printAllItems();
                }

            }
            //Присваем Владельца (нас), по сути, для мультиплеера здесь надо будет setMasterID(Entity->getMasterID());
            //Закидываем в инвентарь
            this->setMasterID(1);
            Entity.insertInInventory(this->getUniqueId());
        }
}


Object::ItemType Object::getItemType() const { return _itemType; }


bool Object::isInInventory() const { return _isInInventory; }

void Object::togleIsInventory() { _isInInventory = !_isInInventory; }


int Object::getItemId() { return _itemID; }

int Object::getUniqueId()
{
    return uniqueId;
}

void Object::setItemId(int ItemId) { _itemID = ItemId; }

void Object::inInventoryDisable()
{
    _isInInventory = false;
}


const sf::Vector2f& Object::getPosition() const { return _position; }

void Object::setPosition(const sf::Vector2f& newPos) { _position = newPos;  }


const sf::Sprite& Object::getSprite() const { return _sprite; }

void Object::setSprite(const sf::Sprite& newSprite) { _sprite = newSprite; }


const std::unordered_set<int>& Object::getEffectIDs() const { return _effectIDs; }

void Object::addEffectID(int effectID) { _effectIDs.insert(effectID); }

void Object::removeEffectID(int effectID) { _effectIDs.erase(effectID); }


void Object::setMasterID(int MasterID) { _masterId = MasterID; }

const int Object::getMasterID() const { return _masterId; }

unsigned int Object::getCost()
{
    return _cost;
}

void Object::setCost(unsigned int cost)
{
    _cost = cost;
}


void Object::Save(const std::string& filename) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл для сохранения: " << filename << std::endl;
        return;
    }

    file << static_cast<int>(_itemType) << " "                      // itemType
        << _position.x/ PIXELS_PER_CELL << " "                      // posX
        << _position.y/ PIXELS_PER_CELL << " "                      // posY


        << static_cast<int>(_isInInventory) << " "                  // isInInventory (1 ili 2)
        << _itemID << " "                                           // itemID

        << _sprite.getTexture()->getNativeHandle() << " "           // спрайт
        << getEffectIDsAsString() << " "                           // effectIDs
        << _masterId << "\n";                                       // MasterId

    file.close();
}



void MouseTake(Character& player,  std::unordered_multiset<Object*>& objects, sf::Vector2f mousePosition) {
    sf::Vector2f playerPosition = player.getPosition();

    for (const auto& obj : objects) 
    {
        if (!obj->isInInventory()) 
        {
            sf::FloatRect objectBounds = obj->getSprite().getGlobalBounds();


            // To collect item zone
            // Change constants on param from constants.hpp
            sf::FloatRect certainedBounds(
                objectBounds.left - 60,
                objectBounds.top - 60,
                objectBounds.width + 120,
                objectBounds.height + 120
            );

            //Если человек рядом, то подбирает предмет
            if (certainedBounds.contains(playerPosition) && certainedBounds.contains(mousePosition)) {
                obj->togleIsInventory();
                obj->setMasterID(1);
                player.insertInInventory(obj->getUniqueId());
            }
        }
    }         
}

void Object::saveAllObjects(const std::string& filename) {
    for (const auto& obj : _allObjects) {
        obj->Save(filename);
    }
}
