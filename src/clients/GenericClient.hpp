#include "IGeneralClient.hpp"
#include "messages/IClientMessage.hpp"
#include <functional>
#include <map>

#define CLASSNAME "GenericClient"

class GenericClient : public IGeneralClient
{
    public:
        GenericClient(IClientMessage* msg) : message(msg) {}

        void processEvent(uint8_t num, const char* payload, size_t length) override
        {
            JsonDocument doc;

            if (nullptr == message) {ESPadapter::print_null(CLASSNAME, __func__); return;}

            if (!message->getDocument(doc, payload)) return;

            for (auto it = map2func.begin(); it != map2func.end(); it++)
            {
                if (message->getMainCommand(doc) == it->first)
                {
                    it->second(num, doc);
                    break;
                }
            }
        }

        void addFunctionToMainCommand
            (std::string key, 
             std::function<void(uint8_t num, JsonDocument& doc)> func) override
        {
            map2func[key] = func;
        }

        int8_t getClientId () const override 
        {   
            if (nullptr != message) return message->getClientId();
            else 
            {
                ESPadapter::print_null(CLASSNAME, __func__);
                return -1;
            }
        }
        std::string getName() const override {return "generic";}

        ~GenericClient() = default;

    private:

        IClientMessage* message = nullptr;
        std::map<std::string, std::function<void(uint8_t num, JsonDocument& doc)>> map2func;
};