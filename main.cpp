#include <iostream>
#include <tgbot/tgbot.h>

int main(int argc, char* argv[]){
    if(argc!= 2){
        std::cerr<<"usage: ./"<<argv[0]<<" [bot api key]\n";
        return 1;
    }

    const auto key = std::string(argv[1]);
    std::cout<<"key provided:"<<key<<"\n";
    TgBot::CurlHttpClient curl_client;
    TgBot::Bot bot(key, curl_client);

    const auto start_cmd = "start";
    const auto hello_cmd = "getHello";
    bot.getEvents().onCommand(start_cmd,
        [&bot](TgBot::Message::Ptr message) {
            auto start_message = "Hello and welcome to HelloWorld bot by me!\n"
                "type /getHello to recieve greetings";
            bot.getApi().sendMessage(message->chat->id, start_message);
    });
    bot.getEvents().onCommand(hello_cmd,
        [&bot](TgBot::Message::Ptr message){
            const auto uname = message->chat->username;
            const auto hello_world = "Have a nice day, "+uname;
            bot.getApi().sendMessage(message->chat->id, hello_world);
        }
    );
    bot.getEvents().onAnyMessage(
        [&bot](TgBot::Message::Ptr message) {
            std::cout<<"User "<<message->chat->username<<" wrote:"<<message->text<<"\n";
    });
    try {
        auto bot_uname = bot.getApi().getMe()->username;
        std::cout<< "Bot username: "<<bot_uname<<"\n";
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            longPoll.start();
        }
    } catch (TgBot::TgException& e) {
        std::cout<<"ERROR: "<<e.what()<<std::endl;
    }
    return 0;
}