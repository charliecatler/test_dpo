#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <vector>


using boost::asio::ip::tcp;

void session(std::shared_ptr<tcp::socket> socket) {
    // auto buffer = std::make_shared<std::vector<char>>(1024); //Буффер для хранения данных клиента
    auto buffer = std::make_shared<std::vector<char>>(10); //Буффер для хранения данных клиента
    // Асихронное чтение из сокета
    socket->async_read_some(boost::asio::buffer(*buffer),
        [socket, buffer](const boost::system::error_code& ec, std::size_t length){
            if (!ec) {
                std::cout << "Received: " << std::string(buffer->data(), length) << std::endl;
                // Отправка обратно данных (эхо ответ)
                boost::asio::async_write(*socket, boost::asio::buffer(*buffer, length),
                    [socket, buffer](const boost::system::error_code& ec, std::size_t /*length*/) {
                if (!ec){ // Если ошибок при записи нет - запускаем сессию на следующий пакет
                    session(socket);
                }
});
            }
        });
}


int main() {
    try {
        boost::asio::io_context io_context; // Объект синхронизации
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 12345)); // Создаем acceptor для прослушивания
        std::cout << "Server running on port 12345\n";
        std::function<void()> do_accept; // Вектор для лямбда-функций для приема новых соединений; ниже - функция
        do_accept = [&]() {
            auto socket = std::make_shared<tcp::socket>(io_context);// Новый сокет для приема
            acceptor.async_accept(*socket, [&, socket](boost::system::error_code ec) { // Асинхроное ожидание соединения
            if (!ec) {
                std::cout << "New client connected\n";
                session(socket);
            }
            do_accept(); // бесконечное соединение
            });
        };
        do_accept(); // Запуск бесконечного цикла асинхронных операций
        io_context.run(); // Блокировка в ожидании завершения асинхронных операций
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
