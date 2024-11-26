#include "MapObject.hpp"
std::unordered_set<MapObject*> MapObject::_allMapObjects;

void Storage::Use()
{
    UIManager* _manager = UIManager::getController();
    _manager->setCommutatorID(masterId);
    _manager->togleWindow(1);
    _manager->togleWindow(5);
  

}

void Storage::insertItem(int itemID)
{
    storage_inventory.insert(itemID);
}

void Storage::DeleteItem(int itemId)
{
    auto it = storage_inventory.find(itemId);
    if (it != storage_inventory.end()) {
        storage_inventory.erase(it);
    }
}

std::multiset<int> Storage::GetInventory()
{
    return storage_inventory;
}

void Storage::printAllItems()
{
    for (auto k : storage_inventory)
    {
        std::cout << "Item id" << k << std::endl;
    }
}

std::unordered_set<MapObject*> MapObject::getAllMapObjects()
{
	return _allMapObjects;
}

int MapObject::getMasterID()
{
    return masterId;
}

// Отображение контекстного меню с текстом

 void Table::ShowContextMenu()
{
    UIManager* _manager = UIManager::getController();
    _manager->togleWindow(4);
    _manager->ChangeText(text);

}

 //Выгрзука MapObjects(массива)
 void LoadMapObjects(const std::string& filePath)
 {

     std::ifstream file(filePath);
     if (!file.is_open())
     {
         throw std::runtime_error("Could not open file: " + filePath);
     }

     std::vector<std::string> dataLines;
     std::string line;
     while (std::getline(file, line))
     {
         dataLines.push_back(line);
     }
     file.close();

     for (size_t i = 0; i < dataLines.size(); ++i)
     {
         std::stringstream ss(dataLines[i]);
         sf::Vector2f position;
         std::string type;

         ss >> position.x >> position.y;
         ss >> type;
         position.x = position.x * PIXELS_PER_CELL;
         position.y = position.y * PIXELS_PER_CELL;

         std::string remainingData;
         std::getline(ss, remainingData); 

         MapObject* obj = nullptr;
         if (type == "Table")
         {
             std::string tableText;
             std::string linePart;

             while (i + 1 < dataLines.size())
             {
                 ++i; 
                 linePart = dataLines[i]; 

                 if (linePart.find('$') != std::string::npos)
                 {
                     tableText += linePart.substr(0, linePart.find('$'));
                     break; 
                 }

                 tableText += linePart + '\n'; 
             }

             obj = new Table(position, tableText);
         }
         else if (type == "Storage")
         {
             std::unordered_multiset<Object*> AllObj = Object::getAllObjects();

             std::multiset<int> storage_inventory;
             std::string idString = remainingData;
             std::stringstream idStream(idString);
             std::string id;
             std::string strMasterID;
             idStream >> strMasterID;
             while (std::getline(idStream, id, ','))
             {
                 int temp = stoi(id);
                 for (auto obj : AllObj)
                 {
                     if (obj->getItemId() == temp)
                     {
                         Object* newObj = new Object(*obj);
                         newObj->setMasterID(stoi(strMasterID));
                         newObj->togleIsInventory();
                         storage_inventory.insert(newObj->getUniqueId());
                     }
                 }
             }
             int MasterID = std::stoi(strMasterID);
             obj = new Storage(position, storage_inventory, MasterID);
         }
         else
         {
             throw std::invalid_argument("Cho za object broo");
         }

         MapObject::_allMapObjects.insert(obj);
     }
 }

 void ActivateMapObj(Character& player, sf::Vector2f mousePos, std::unordered_set<MapObject*> _allMapObjects)
 {

     for (auto mapObj : _allMapObjects) {
         if (mapObj->IsMouseOverTable(mousePos) && mapObj->IsPlayerInRange(player))
         {
             mapObj->Use();

         }
     }
 }
