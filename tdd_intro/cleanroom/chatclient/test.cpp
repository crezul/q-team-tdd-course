﻿#include <gtest/gtest.h>

using namespace testing;
/*
Implement chat application, that communicates via TCP sockets.
 There is list of requirenments to this application:
 * It receives user nickname through commandline arguments on start
 * It runs only for two clients
 * On start it checks if port 4444 is bound
    * if it is - connects to other client
    * if not - binds a port and waits for inbound connection
        * message "No one is here..." is displayed in UI
 * After establishing connection - handshake is performed:
    * client writes message with it's nickname and ':HELLO!' string to socket ("client:HELLO!")
    * server responses with it's nickname and ':HELLO!' magic ("server:HELLO!")
        * if server receives malformated message - it drops connection with this client
        * if client receives malformated message - it drops connection and exits
            * Error message is shown
    * Listening socket must be closed
        (i) More than one client is not allowed to connect

 * It must send messages via created connection, until it is dropped
    * Application outputs "me: " prefix for user input
    * User inputs message through console interface and presses ENTER
    * Text message is sent to socket
 * It must receive messages via created connection, until it is dropped
    * Message received through socket
    * Line with message and '@friend_name: ' prefix is displayed ("metizik: Hello!")
 * END of message is determined by '\0' byte
 * When connection is dropped by one of users
    * "You are alone now" message is displayed to user
    * Application exits

  (i) Client starts dialog

 OPTIONAL requirement:
    * If user enters '!exit!' message, application must close connection and exit
    * If user runs app with 'me' nickname - error with text "Username me is reserved and can not be used" is displayed and application exits
*/

#include "mocks.h"
#include "utils.h"
#include "connector.h"

using namespace ::testing;


TEST(Chat, StartAsServer)
{
    StrictMock<SocketWrapperMock> socketMock;
    EXPECT_CALL(socketMock, Bind(_, _));
    ASSERT_TRUE(utils::TryToBind(socketMock));
}

TEST(Chat, StartAsClient)
{
    StrictMock<SocketWrapperMock> socketMock;
    EXPECT_CALL(socketMock, Bind(_, _)).WillOnce(Throw(std::runtime_error("")));
    ASSERT_FALSE(utils::TryToBind(socketMock));
}

TEST(Chat, StartConnection)
{
    StrictMock<SocketWrapperMock> socketMock;
    EXPECT_CALL(socketMock, Bind(_, _)).WillOnce(Throw(std::runtime_error("")));
    EXPECT_CALL(socketMock, Connect(_, _)).WillOnce(Return(ISocketWrapperPtr()));
    bool isServer = false;
    utils::EstablishConnection(socketMock, isServer);
}

TEST(Chat, AcceptAfterListen)
{
    StrictMock<SocketWrapperMock> listener;
    StrictMock<SocketWrapperMock> client;
    InSequence sequence;
    EXPECT_CALL(listener, Bind(_, _));
    EXPECT_CALL(listener, Listen());
    EXPECT_CALL(listener, Accept()).WillOnce(Return(ISocketWrapperPtr()));
    EXPECT_CALL(client, Bind(_, _)).WillOnce(Throw(std::runtime_error("")));
    EXPECT_CALL(client, Connect(_, _)).WillOnce(Return(ISocketWrapperPtr()));

    bool isServer = false;
    utils::EstablishConnection(listener, isServer);
    ASSERT_TRUE(isServer);
    utils::EstablishConnection(client, isServer);
    ASSERT_FALSE(isServer);
}

TEST(Chat, ReturnsClientSocket)
{
    SocketWrapperMock listener;
    EXPECT_CALL(listener, Accept()).WillOnce(Return(std::make_shared<SocketWrapperMock>()));
    bool isServer = false;
    ASSERT_NE(nullptr, utils::EstablishConnection(listener, isServer));
}

TEST(Chat, ReturnsServerSocket)
{
    SocketWrapperMock server;
    EXPECT_CALL(server, Bind(_, _)).WillOnce(Throw(std::runtime_error("")));
    EXPECT_CALL(server, Connect(_, _)).WillOnce(Return(std::make_shared<SocketWrapperMock>()));
    bool isServer = false;
    ASSERT_NE(nullptr, utils::EstablishConnection(server, isServer));
}

TEST(Chat, WriteSomethingToSocket)
{
    SocketWrapperMock server;
    EXPECT_CALL(server, Write(_)).Times(1);
    utils::WriteToSocket(server,"Hello");
}

TEST(Chat, WriteDataToSocket)
{
    std::string expected("Hello", sizeof("Hello"));

    SocketWrapperMock server;
    EXPECT_CALL(server, Write(expected)).Times(1);
    utils::WriteToSocket(server,"Hello");
}

TEST(Chat, ReadSomethingFromSocket)
{
    SocketWrapperMock server;
    std::string data;
    EXPECT_CALL(server, Read(data)).Times(1);
    utils::ReadFromSocket(server, data);
}

TEST(Chat, ReadDataFromSocket)
{
    SocketWrapperMock server;
    std::string data;
    EXPECT_CALL(server, Read(data)).WillOnce(::SetArgReferee<0>("Hello"));
    utils::ReadFromSocket(server, data);
    ASSERT_EQ("Hello", data);
}

TEST(Chat, ClientHandshakeAnswer)
{
    SocketWrapperMock socket;
    std::string nickname = "client";
    EXPECT_CALL(socket, Write("client:HELLO!")).Times(1);
    EXPECT_CALL(socket, Read(_)).WillOnce(::SetArgReferee<0>("server:HELLO!"));
    utils::ClientHandshake(socket, nickname);
}

TEST(Chat, ClientInvalidHandshakeAnswer)
{
    SocketWrapperMock socket;
    std::string nickname = "client";
    EXPECT_CALL(socket, Write("client:HELLO!")).Times(1);
    EXPECT_CALL(socket, Read(_)).WillOnce(::SetArgReferee<0>("HELLO!"));
    EXPECT_ANY_THROW(utils::ClientHandshake(socket, nickname));
}

TEST(Chat, ClientHandshakeReturnsServerNickname)
{
    SocketWrapperMock socket;
    std::string nickname = "client";
    EXPECT_CALL(socket, Write("client:HELLO!")).Times(1);
    EXPECT_CALL(socket, Read(_)).WillOnce(::SetArgReferee<0>("Bob:HELLO!"));
    EXPECT_EQ("Bob", utils::ClientHandshake(socket, nickname));
}

TEST(Chat, ServerHandshake)
{
    SocketWrapperMock socket;
    std::string nickname = "server";
    InSequence sequence;
    EXPECT_CALL(socket, Read(_)).WillOnce(::SetArgReferee<0>("client:HELLO!"));
    EXPECT_CALL(socket, Write("server:HELLO!")).Times(1);
    utils::ServerHandshake(socket, nickname);
}

TEST(Chat, ServerHandshakeReturnsClientNickname)
{
    SocketWrapperMock socket;
    std::string nickname = "Bob";
    InSequence sequence;
    EXPECT_CALL(socket, Read(_)).WillOnce(::SetArgReferee<0>("Alice:HELLO!"));
    EXPECT_CALL(socket, Write("Bob:HELLO!")).Times(1);
    EXPECT_EQ("Alice", utils::ServerHandshake(socket, nickname));
}

TEST(Chat, ServerInvalidHandshakeAnswer)
{
    StrictMock<SocketWrapperMock> socket;
    std::string nickname = "server";
    EXPECT_CALL(socket, Read(_)).WillOnce(::SetArgReferee<0>("HELLO!"));
    EXPECT_ANY_THROW(utils::ServerHandshake(socket, nickname));
}

TEST(Chat, ClientEstablishesConnection)
{
    SocketWrapperMock socket;
    std::shared_ptr<SocketWrapperMock> companion(new SocketWrapperMock());
    std::string nickname = "Bob";
    EXPECT_CALL(socket, Bind(_, _)).WillOnce(Throw(std::runtime_error("")));
    EXPECT_CALL(socket, Connect(_, _)).WillOnce(Return(companion));
    EXPECT_CALL(*companion, Read(_)).WillOnce(::SetArgReferee<0>("Alice:HELLO!"));
    Connector client(socket, nickname);
    EXPECT_EQ(client.GetCompanionNickname(), "Alice");
}

TEST(Chat, ServerEstablishesConnection)
{

    SocketWrapperMock socket;
    std::shared_ptr<SocketWrapperMock> companion(new SocketWrapperMock());
    std::string nickname = "Bob";

   // InSequence seq; TO DO fix leak
    EXPECT_CALL(socket, Accept()).WillOnce(Return(companion));
    EXPECT_CALL(*companion, Read(_)).WillOnce(::SetArgReferee<0>("Alice:HELLO!"));
    EXPECT_CALL(*companion, Write("Bob:HELLO!"));
    Connector client(socket, nickname);
    EXPECT_EQ(client.GetCompanionNickname(), "Alice");
}

TEST(Chat, SendMessage)
{
    SocketWrapperMock socket;
    GuiMock gui;
    EXPECT_CALL(gui, Read()).WillOnce(Return("some info"));
    EXPECT_CALL(socket, Write("some info"));

    utils::WriteFromGuiToSocket(gui, socket);
}

TEST(Chat, ReceiveMessage)
{
    SocketWrapperMock socket;
    GuiMock gui;
    std::string name = "Alice";
    EXPECT_CALL(socket, Read(_)).WillOnce(::SetArgReferee<0>("Info"));
    EXPECT_CALL(gui, Write("Alice: Info"));

    utils::WriteFromSocketToGui(gui, socket, name);
}
