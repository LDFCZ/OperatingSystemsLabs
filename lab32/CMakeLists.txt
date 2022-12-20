cmake_minimum_required(VERSION 3.24)
project(lab32)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(lab32 main.cpp socket/ServerSocketImpl.cpp socket/ServerSocketImpl.h exception/ConnectException.h socket/ServerSocket.h context/cash/CashContext.cpp context/cash/CashContext.h context/thread/ThreadContext.cpp context/thread/ThreadContext.h server/Server.h server/ServerImpl.cpp server/ServerImpl.h client/Client.h client/ClientImpl.cpp client/ClientImpl.h Consts.h logger/Logger.h cleaner/Cleaner.cpp cleaner/Cleaner.h buffer/Buffer.h buffer/BufferImpl.cpp buffer/BufferImpl.h context/Context.cpp context/Context.h exception/ParseException.h parser/ParserImpl.cpp exception/CashContextException.h)
