#ifndef JSON_SERIALIZABLE
#define JSON_SERIALIZABLE

#include <json/json.h>

class IJsonSerializable
{
public:
   virtual ~IJsonSerializable( void ) {};
   virtual void Serialize( Json::Value& root ) =0;
   virtual void Deserialize( Json::Value& root) =0;
};

class CJsonSerializer
{
public:
   static bool Serialize( IJsonSerializable* pObj, std::string& output );
   static bool Deserialize( IJsonSerializable* pObj, std::string& input );

private:
   CJsonSerializer( void ) {};
};

#endif
