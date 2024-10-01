#include "Object.hpp"

 std::unordered_set<Object*> Object::allObjects;


 std::unordered_set<Object*>& Object::getAllObjects() {
    return allObjects;
}

 void Object::Load(const std::string& filename) {
     std::ifstream file(filename);
     if (!file.is_open()) {
         std::cerr << " не удалось открыть файл: " << filename << std::endl;
         return;
     }

     std::string line;
     while (std::getline(file, line)) {
         std::istringstream iss(line);

         int itemTypeInt, itemID, masterID;
         float posX, posY, sizeX, sizeY;
         bool isInInventory;
         std::string spritePath, effectIDsStr;

         if (!(iss >> itemTypeInt >> posX >> posY  >> isInInventory >> itemID >> spritePath >> effectIDsStr >> masterID)) {
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
         Object* newObj = new Object(itemType, pos, sprite, isInInventory, itemID, effectIDs);
         newObj->setMasterID(masterID);

  
   
         allObjects.insert(newObj);
     }

     file.close();
 }


 std::string Object::getEffectIDsAsString()  {
     std::ostringstream oss;

     for (const auto& id : effectIDs) {
         oss << id << ",";
     }

     std::string result = oss.str();
     
     if (
!result.empty()) {
     
         result.pop_back(); 
     }
     return result;
 }





 bool Object::getIsInInventory() const { return isInInventory; }

  void Object::setIsInInventory(bool inInventory) { isInInventory = inInventory; }

  void Object::setItemId(int ItemId) { itemID = ItemId; }

  int Object::getItemId() { return itemID; }


  const sf::Vector2f& Object::getPosition() const { return position; }

  void Object::setPosition(const sf::Vector2f& newPos) { position = newPos; }

  const sf::Sprite& Object::getSprite() const { return sprite; }

 void Object::setSprite(const sf::Sprite& newSprite) { sprite = newSprite; }

 const std::unordered_set<int>& Object::getEffectIDs() const { return effectIDs; }

 void Object::addEffectID(int effectID) { effectIDs.insert(effectID); }

 void Object::removeEffectID(int effectID) { effectIDs.erase(effectID); }

 void Object::setMasterID(int MasterID) { MasterId = MasterID; }

 const int Object::getMasterID() const { return MasterId; }


 void Object::Save(const std::string& filename) {
     std::ofstream file(filename, std::ios::app);
     if (!file.is_open()) {
         std::cerr << "Не удалось открыть файл для сохранения: " << filename << std::endl;
         return;
     }

     file << static_cast<int>(itemType) << " "     // itemType
         << position.x/ PIXELS_PER_CELL << " "                      // posX
         << position.y/ PIXELS_PER_CELL << " "                      // posY


         << static_cast<int>(isInInventory) << " " // isInInventory (1 ili 2)
         << itemID << " "                          // itemID

         << sprite.getTexture()->getNativeHandle() << " " // спрайт
         << getEffectIDsAsString() << " "           // effectIDs
         << MasterId << "\n";                       // MasterId

     file.close();
 }



 void MouseTake(Player& player,  std::unordered_set<Object*>& objects, sf::Vector2f mousePosition) {
     sf::Vector2f playerPosition = player.getPosition();

     for (const auto& obj : objects) {
         if (!obj->getIsInInventory()) {
             sf::FloatRect objectBounds = obj->getSprite().getGlobalBounds();
             sf::FloatRect extendedBounds(
                 objectBounds.left - 60,
                 objectBounds.top - 60,
                 objectBounds.width + 120,
                 objectBounds.height + 120
             );
             if (extendedBounds.contains(playerPosition) && extendedBounds.contains(mousePosition)) {
                 obj->setIsInInventory(true);
                 obj->setMasterID(1);
                 player.insertInInventory(obj->getItemId());
             }
         }
     }
             
 }

 void saveAllObjects(const std::string& filename) {
     const auto& allObjects = Object::getAllObjects();
     for (const auto& obj : allObjects) {
         obj->Save(filename);
     }
 }
