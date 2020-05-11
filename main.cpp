#include <iostream>
#include <tgbot/tgbot.h>

int main(int argc, char* argv[]){
    if(argc!= 2){
        std::cerr<<"usage: ./"<<argv[0]<<" [bot api key]\n";
        return 1;
    }

    const auto key = std::string(argv[1]);
    std::cout<<"key provided:"<<key<<"\n";
    TgBot::Bot bot(key, TgBot::CurlHttpClient());
    std::cout<<"key in tglib:"<<bot.getToken()<<"\n";
    bot.getEvents().onCommand("start",
        [&bot](TgBot::Message::Ptr message) {
            auto start_message = "Hello and welcome to HelloWorld bot by me!\n"
                "type /getHello to recieve greetings";
            bot.getApi().sendMessage(message->chat->id, start_message);
    });
    bot.getEvents().onCommand("getHello",
        [&bot](TgBot::Message::Ptr message){
            const auto uname = message->chat->username;
            const auto hello_world = "Have a nice day, "+uname;
            bot.getApi().sendMessage(message->chat->id, hello_world);
        }
    );
    bot.getEvents().onAnyMessage(
        [&bot](TgBot::Message::Ptr message) {
            std::cout<<"User wrote:"<<message->text<<"\n";
            if (StringTools::startsWith(message->text, "/start")) {
                return;
            }
        auto chat_id = message->chat->id;
        auto mes = "Your message is: " + message->text;
        bot.getApi().sendMessage(chat_id, mes);
    });
    try {
        auto bot_uname = bot.getApi().getMe()->username;
        std::cout<< "Bot username: "<<bot_uname<<"\n";
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            std::cout<<"Long poll started\n";
            longPoll.start();
        }
    } catch (TgBot::TgException& e) {
        std::cout<<"ERROR: "<<e.what()<<std::endl;
    }
    return 0;
}