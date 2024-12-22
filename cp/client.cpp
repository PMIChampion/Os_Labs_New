// client.cpp
#include <iostream>
#include <string>
#include <zmq.hpp>

static void sendRequest(zmq::socket_t &socket, const std::string &req) {
    zmq::message_t request(req.size());
    memcpy((void*)request.data(), req.data(), req.size());
    socket.send(request, zmq::send_flags::none);
}

static std::string recvReply(zmq::socket_t &socket) {
    zmq::message_t reply;
    socket.recv(&reply);
    std::string rep_str(static_cast<char*>(reply.data()), reply.size());
    return rep_str;
}

int main() {
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);
    socket.connect("tcp://localhost:5555");

    std::cout << "Введите ваш логин: ";
    std::string login;
    std::cin >> login;

    {
        std::string req = "LOGIN:" + login;
        sendRequest(socket, req);
        std::string rep_str = recvReply(socket);
        if (rep_str.find("OK:") != 0) {
            std::cout << "Ошибка входа: " << rep_str << std::endl;
            return 1;
        } else {
            std::cout << "Успешный вход: " << rep_str << std::endl;
        }
    }

    bool inGame = false;
    bool gameStarted = false;
    std::string currentGame;

    while (true) {
        if (!inGame) {
            std::cout << "\nВы вне игры. Доступные действия:\n";
            std::cout << "1) CREATE_GAME\n";
            std::cout << "2) JOIN_GAME\n";
            std::cout << "3) LIST_GAMES\n";
            std::cout << "4) INVITE\n";
            std::cout << "5) QUIT (выйти из программы)\n";
            int choice;
            std::cin >> choice;

            if (choice == 5) {
                break;
            }

            std::string req;
            if (choice == 1) {
                std::cout << "Введите имя новой игры: ";
                std::string gname;
                std::cin >> gname;
                req = "CREATE_GAME:" + gname + "|" + login;
            } else if (choice == 2) {
                std::cout << "Введите имя игры для подключения: ";
                std::string gname;
                std::cin >> gname;
                req = "JOIN_GAME:" + gname + "|" + login;
            } else if (choice == 3) {
                req = "LIST_GAMES:" + login;
            } else if (choice == 4) {
                std::cout << "Введите логин для приглашения: ";
                std::string other;
                std::cin >> other;
                req = "INVITE:" + other + "|" + login;
            } else {
                std::cout << "Неизвестный выбор.\n";
                continue;
            }

            sendRequest(socket, req);
            std::string rep = recvReply(socket);
            std::cout << "Ответ сервера: " << rep << std::endl;
            if (rep.find("OK:GAME_CREATED") == 0 || rep.find("OK:JOINED") == 0) {
                inGame = true;
                if (req.find("CREATE_GAME:") == 0) {
                    currentGame = req.substr(std::string("CREATE_GAME:").size());
                    currentGame = currentGame.substr(0, currentGame.find('|'));
                } else if (req.find("JOIN_GAME:") == 0) {
                    currentGame = req.substr(std::string("JOIN_GAME:").size());
                    currentGame = currentGame.substr(0, currentGame.find('|'));
                }
            }
        } else {
            // Получаем статус игры
            {
                std::string req = "GAME_STATUS:" + login;
                sendRequest(socket, req);
                std::string rep = recvReply(socket);
                if (rep.find("OK:") == 0) {
                    if (rep.find("STARTED") != std::string::npos) {
                        gameStarted = true;
                    } else {
                        gameStarted = false;
                    }
                    std::cout << "\n--- Состояние игры ---\n" << rep << "\n----------------------\n";
                } else {
                    std::cout << "Ошибка получения статуса: " << rep << std::endl;
                }
            }

            if (!gameStarted) {
                // Расставляем корабли
                // Нам нужно 3 раза расставить корабли: 1x4, 2x3
                std::cout << "\nИгра не началась. Вам нужно расставить 3 корабля.\n";
                std::cout << "Введите 1 чтобы поставить корабль, 2 чтобы выйти из игры: ";
                int choice;
                std::cin >> choice;
                if (choice == 2) {
                    inGame = false;
                    currentGame.clear();
                    continue;
                }
                if (choice == 1) {
                    std::cout << "Введите координаты корабля (startX startY endX endY): ";
                    int sx, sy, ex, ey;
                    std::cin >> sx >> sy >> ex >> ey;
                    std::string req = "SETUP_SHIP:" + std::to_string(sx) + "," + std::to_string(sy) + "," +
                                      std::to_string(ex) + "," + std::to_string(ey) + "|" + login + "|" + currentGame;
                    sendRequest(socket, req);
                    std::string rep = recvReply(socket);
                    std::cout << "Ответ сервера: " << rep << std::endl;
                }

            } else {
                // Игра началась
                std::cout << "\nИгра идет. Доступные действия:\n";
                std::cout << "1) MOVE (сделать выстрел)\n";
                std::cout << "2) GAME_STATUS (посмотреть состояние)\n";
                std::cout << "3) QUIT_GAME (вернуться в главное меню)\n";
                int choice;
                std::cin >> choice;

                if (choice == 3) {
                    inGame = false;
                    currentGame.clear();
                    continue;
                } else if (choice == 2) {
                    std::string req = "GAME_STATUS:" + login;
                    sendRequest(socket, req);
                    std::string rep = recvReply(socket);
                    std::cout << "Ответ сервера:\n" << rep << std::endl;
                } else if (choice == 1) {
                    std::cout << "Введите координаты выстрела x y: ";
                    int x,y;
                    std::cin >> x >> y;
                    std::string req = "MOVE:" + std::to_string(x) + "," + std::to_string(y) + "|" + login + "|" + currentGame;
                    sendRequest(socket, req);
                    std::string rep = recvReply(socket);
                    std::cout << "Ответ сервера: " << rep << std::endl;
                    // Если это был WIN - игра окончена
                    if (rep.find("WIN") != std::string::npos) {
                        std::cout << "Вы победили! Игра окончена.\n";
                        inGame = false;
                        currentGame.clear();
                        gameStarted = false;
                    }
                } else {
                    std::cout << "Неизвестный выбор.\n";
                }
            }
        }
    }

    std::cout << "Выход.\n";
    return 0;
}
