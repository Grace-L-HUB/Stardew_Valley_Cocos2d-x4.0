#ifndef  _APP_DELEGATE_H_  // ·ÀÖ¹Í·ÎÄ¼ş±»¶à´Î°üº¬µÄºê
#define  _APP_DELEGATE_H_

#include "Timesystem.h"
#include "cocos2d.h"  
#include "audio/include/AudioEngine.h" 
//#include "GameBeginUI.h"
#include "Barn.h"
#include "Beach.h"
#include "Cave.h"
#include "farm.h"
#include "Forest.h"
#include "Myhouse.h"
#include "supermarket.h"
#include "Town.h"

#include "Player.h"

#include "mini_bag.h"
#include "Inventory.h"
#include "InventoryUI.h"

#include "Item.h"
#include "Generaltem.h"

#include "BasicInformation.h"
#include "Crop.h"
#include "Ore.h"
#include "tree.h"

#include "Livestock.h"
#include "Chicken.h"
#include "Cow.h"
#include "Sheep.h"
#include "TaskManagement.h"
#include "NPC.h"
#include "NPCreate.h"
#include "NPCtalkUI.h"
#include "NpcRelationship.h"
#include "intimacyUI.h"
#include "TaskManagement.h"
#include "StoreUI.h"
#include "EconomicSystem.h"
#include "SkillTree.h"
#include <memory>

#define Daytime 43200

USING_NS_CC;

class Player;
class Town;
class farm;
class supermarket;
class Myhouse;
class Crop;
class Item;
class CropBasicInformation;
class Barn;
class Inventory;
class NpcRelationship;
class mini_bag;
class TaskManagement;
class Timesystem;
/******************************** È«¾Ö±äÁ¿ÉùÃ÷Çø ***************************************/
extern int remainingTime;  // ÉùÃ÷£¬²»³õÊ¼»¯
extern int day;
extern int strength;
extern int GoldAmount;
extern bool IsNextDay;
extern bool frombed;
extern bool IsSleep;
extern bool GoldMaskfirst;
extern bool RainBowfirst;
extern std::string Season;
extern std::string Weather;
extern std::string Festival;
extern std::map<std::string , int> season;
// Ã¿ÖÖ×÷ÎïµÄ»ù±¾ĞÅÏ¢
extern std::map<std::string , Crop> cropbasicinformation;
extern std::map<std::pair<std::string , Vec2> , bool> T_lastplace;
extern std::map<std::pair<std::string, Vec2>, bool> F_lastplace;
extern std::map<std::pair<std::string, Vec2>, bool> W_lastplace;
// ÒÑ¾­ÖÖÖ²µÄ×÷ÎïµÄ×´Ì¬ĞÅÏ¢
extern std::vector<std::shared_ptr<Crop>> Crop_information;
extern std::vector<std::shared_ptr<Ore>> Ore_information; 
extern std::vector<std::shared_ptr<Tree>> Tree_information; 
// ³õÊ¼»¯¸÷×÷ÎïµÄ»ù±¾ĞÅÏ¢
extern CropBasicInformation WHEAT;
extern CropBasicInformation CORN;
extern CropBasicInformation POTATO;
extern CropBasicInformation PUMPKIN;
extern CropBasicInformation BLUEBERRY;
extern Crop wheat; 
extern Crop corn;
extern Crop potato;
extern Crop pumpkin;
extern Crop blueberry;

//ÈËÎï»ù±¾ĞÅÏ¢
extern std::string protagonistName;
extern std::string FarmName;

extern Player* player1;  // ÉùÃ÷Ö¸Õë±äÁ¿£¬²»³õÊ¼»¯
extern SkillTree* skill_tree;
extern mini_bag* miniBag;
extern Inventory* inventory;
extern NpcRelationship* npc_relationship;
extern Inventory* StoreItem;
extern TaskManagement* taskManager;
extern Timesystem* TimeUI;
//´æ´¢ĞóÅïÖĞ¿ÉÓĞ¼ÒĞó»î¶¯µÄ¾ØĞÎÇøÓò£¬²¢¼ÇÂ¼¸ÃÇøÓòÊÇ·ñÒÑ´æÔÚ¼ÒĞó
extern std::vector<std::pair<Rect , bool>> barn_space;
//´æ´¢ĞóÅïÖĞµÄ¼ÒĞó
extern std::vector<Livestock*> livestocks;
/**************************************************************************************/


 /**
  @brief    Cocos2d Ó¦ÓÃ³ÌĞòµÄÎ¯ÍĞÀà£¨AppDelegate£©

  ´ËÀà¸ºÔğ´¦ÀíÓ¦ÓÃ³ÌĞòµÄÉúÃüÖÜÆÚ¹ÜÀí£¬°üÀ¨Ó¦ÓÃÆô¶¯¡¢½øÈëºóÌ¨¡¢½øÈëÇ°Ì¨µÈ²Ù×÷¡£
  ¼Ì³Ğ×Ô cocos2d::Application Àà£¬µ«Í¨¹ıË½ÓĞ¼Ì³ĞÀ´Òş²Ø²¿·Ö½Ó¿Ú£¬È·±£ÓÃ»§Ö»ÄÜ·ÃÎÊ¹«¹²½Ó¿Ú¡£
  */

  /************************************ È«¾Ö±äÁ¿ÉùÃ÷Çø ******************************************/
  // Éè¶¨ÓÎÏ·»­Ãæ´óĞ¡£º1600, 1280
static cocos2d::Size designResolutionSize = cocos2d::Size(1600, 1280);  // Éè¼Æ·Ö±æÂÊ


class AppDelegate : private cocos2d::Application  // ¼Ì³Ğ×Ô cocos2d::Application Àà£¬²¢Ë½ÓĞ»¯¼Ì³Ğ
{
public:
    // ¹¹Ôìº¯Êı
    AppDelegate();

    // Îö¹¹º¯Êı
    virtual ~AppDelegate();

    // ³õÊ¼»¯ OpenGL ÉÏÏÂÎÄÊôĞÔµÄº¯Êı
    virtual void initGLContextAttrs();

    /**
    @brief    Ó¦ÓÃ³ÌĞòÆô¶¯Ê±µÄ³õÊ¼»¯º¯Êı
    @return true    ³õÊ¼»¯³É¹¦£¬Ó¦ÓÃ¼ÌĞøÔËĞĞ¡£
    @return false   ³õÊ¼»¯Ê§°Ü£¬Ó¦ÓÃ³ÌĞòÍË³ö¡£
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief    Ó¦ÓÃ³ÌĞò½øÈëºóÌ¨Ê±µ÷ÓÃ¡£
    @param    the pointer of the application Ö¸ÏòÓ¦ÓÃ³ÌĞòµÄÖ¸Õë£¨Cocos2d ÄÚ²¿»áµ÷ÓÃ´Ëº¯Êı£©
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief    Ó¦ÓÃ³ÌĞòÖØĞÂ½øÈëÇ°Ì¨Ê±µ÷ÓÃ¡£
    @param    the pointer of the application Ö¸ÏòÓ¦ÓÃ³ÌĞòµÄÖ¸Õë£¨Cocos2d ÄÚ²¿»áµ÷ÓÃ´Ëº¯Êı£©
    */
    virtual void applicationWillEnterForeground();
#ifndef  _APP_DELEGATE_H_  // é˜²æ­¢å¤´æ–‡ä»¶è¢«å¤šæ¬¡åŒ…å«çš„å®
#define  _APP_DELEGATE_H_

#include "Timesystem.h"
#include "cocos2d.h"  
#include "audio/include/AudioEngine.h" 
//#include "GameBeginUI.h"
#include "Barn.h"
#include "Beach.h"
#include "Cave.h"
#include "farm.h"
#include "Forest.h"
#include "Myhouse.h"
#include "supermarket.h"
#include "Town.h"

#include "Player.h"

#include "mini_bag.h"
#include "Inventory.h"
#include "InventoryUI.h"

#include "Item.h"
#include "Generaltem.h"

#include "BasicInformation.h"
#include "Crop.h"
#include "Ore.h"
#include "tree.h"

#include "Livestock.h"
#include "Chicken.h"
#include "Cow.h"
#include "Sheep.h"
#include "TaskManagement.h"
#include "NPC.h"
#include "NPCreate.h"
#include "NPCtalkUI.h"
#include "NpcRelationship.h"
#include "intimacyUI.h"
#include "TaskManagement.h"
#include "StoreUI.h"
#include "EconomicSystem.h"
#include "SkillTree.h"
#include <memory>

#define Daytime 43200

USING_NS_CC;

class Player;
class Town;
class farm;
class supermarket;
class Myhouse;
class Crop;
class Item;
class CropBasicInformation;
class Barn;
class Inventory;
class NpcRelationship;
class mini_bag;
class TaskManagement;
class Timesystem;
/******************************** å…¨å±€å˜é‡å£°æ˜åŒº ***************************************/
extern int remainingTime;  // å£°æ˜ï¼Œä¸åˆå§‹åŒ–
extern int day;
extern int strength;
extern int GoldAmount;
extern bool IsNextDay;
extern bool frombed;
extern bool IsSleep;
extern bool GoldMaskfirst;
extern bool RainBowfirst;
extern std::string Season;
extern std::string Weather;
extern std::string Festival;
extern std::map<std::string , int> season;
// æ¯ç§ä½œç‰©çš„åŸºæœ¬ä¿¡æ¯
extern std::map<std::string , Crop> cropbasicinformation;
extern std::map<std::pair<std::string , Vec2> , bool> T_lastplace;
extern std::map<std::pair<std::string, Vec2>, bool> F_lastplace;
extern std::map<std::pair<std::string, Vec2>, bool> W_lastplace;
// å·²ç»ç§æ¤çš„ä½œç‰©çš„çŠ¶æ€ä¿¡æ¯
extern std::vector<std::shared_ptr<Crop>> Crop_information;
extern std::vector<std::shared_ptr<Ore>> Ore_information; 
extern std::vector<std::shared_ptr<Tree>> Tree_information; 
// åˆå§‹åŒ–å„ä½œç‰©çš„åŸºæœ¬ä¿¡æ¯
extern CropBasicInformation WHEAT;
extern CropBasicInformation CORN;
extern CropBasicInformation POTATO;
extern CropBasicInformation PUMPKIN;
extern CropBasicInformation BLUEBERRY;
extern Crop wheat; 
extern Crop corn;
extern Crop potato;
extern Crop pumpkin;
extern Crop blueberry;

//äººç‰©åŸºæœ¬ä¿¡æ¯
extern std::string protagonistName;
extern std::string FarmName;

extern Player* player1;  // å£°æ˜æŒ‡é’ˆå˜é‡ï¼Œä¸åˆå§‹åŒ–
extern SkillTree* skill_tree;
extern mini_bag* miniBag;
extern Inventory* inventory;
extern NpcRelationship* npc_relationship;
extern Inventory* StoreItem;
extern TaskManagement* taskManager;
extern Timesystem* TimeUI;
//å­˜å‚¨ç•œæ£šä¸­å¯æœ‰å®¶ç•œæ´»åŠ¨çš„çŸ©å½¢åŒºåŸŸï¼Œå¹¶è®°å½•è¯¥åŒºåŸŸæ˜¯å¦å·²å­˜åœ¨å®¶ç•œ
extern std::vector<std::pair<Rect , bool>> barn_space;
//å­˜å‚¨ç•œæ£šä¸­çš„å®¶ç•œ
extern std::vector<Livestock*> livestocks;
/**************************************************************************************/


 /**
  @brief    Cocos2d åº”ç”¨ç¨‹åºçš„å§”æ‰˜ç±»ï¼ˆAppDelegateï¼‰

  æ­¤ç±»è´Ÿè´£å¤„ç†åº”ç”¨ç¨‹åºçš„ç”Ÿå‘½å‘¨æœŸç®¡ç†ï¼ŒåŒ…æ‹¬åº”ç”¨å¯åŠ¨ã€è¿›å…¥åå°ã€è¿›å…¥å‰å°ç­‰æ“ä½œã€‚
  ç»§æ‰¿è‡ª cocos2d::Application ç±»ï¼Œä½†é€šè¿‡ç§æœ‰ç»§æ‰¿æ¥éšè—éƒ¨åˆ†æ¥å£ï¼Œç¡®ä¿ç”¨æˆ·åªèƒ½è®¿é—®å…¬å…±æ¥å£ã€‚
  */

  /************************************ å…¨å±€å˜é‡å£°æ˜åŒº ******************************************/
  // è®¾å®šæ¸¸æˆç”»é¢å¤§å°ï¼š1600, 1280
static cocos2d::Size designResolutionSize = cocos2d::Size(1600, 1280);  // è®¾è®¡åˆ†è¾¨ç‡


class AppDelegate : private cocos2d::Application  // ç»§æ‰¿è‡ª cocos2d::Application ç±»ï¼Œå¹¶ç§æœ‰åŒ–ç»§æ‰¿
{
public:
    // æ„é€ å‡½æ•°
    AppDelegate();

    // ææ„å‡½æ•°
    virtual ~AppDelegate();

    // åˆå§‹åŒ– OpenGL ä¸Šä¸‹æ–‡å±æ€§çš„å‡½æ•°
    virtual void initGLContextAttrs();

    /**
    @brief    åº”ç”¨ç¨‹åºå¯åŠ¨æ—¶çš„åˆå§‹åŒ–å‡½æ•°
    @return true    åˆå§‹åŒ–æˆåŠŸï¼Œåº”ç”¨ç»§ç»­è¿è¡Œã€‚
    @return false   åˆå§‹åŒ–å¤±è´¥ï¼Œåº”ç”¨ç¨‹åºé€€å‡ºã€‚
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief    åº”ç”¨ç¨‹åºè¿›å…¥åå°æ—¶è°ƒç”¨ã€‚
    @param    the pointer of the application æŒ‡å‘åº”ç”¨ç¨‹åºçš„æŒ‡é’ˆï¼ˆCocos2d å†…éƒ¨ä¼šè°ƒç”¨æ­¤å‡½æ•°ï¼‰
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief    åº”ç”¨ç¨‹åºé‡æ–°è¿›å…¥å‰å°æ—¶è°ƒç”¨ã€‚
    @param    the pointer of the application æŒ‡å‘åº”ç”¨ç¨‹åºçš„æŒ‡é’ˆï¼ˆCocos2d å†…éƒ¨ä¼šè°ƒç”¨æ­¤å‡½æ•°ï¼‰
    */
    virtual void applicationWillEnterForeground();

    void runScene(cocos2d::Director* director);

    void Initialize();


};

#endif  _APP_DELEGATE_H_  // ½áÊø·ÀÖ¹Í·ÎÄ¼ş±»¶à´Î°üº¬µÄºê
