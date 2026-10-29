#pragma once

#ifndef __EMSCRIPTEN__
#include "zmq.hpp"
#endif

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/thread.hpp>
#include <godot_cpp/classes/mutex.hpp>

#include <memory>

namespace godot {

class ZMQSender : public Node
{
    GDCLASS(ZMQSender, Node);

protected:
    static void _bind_methods();
    void beginReceiveThread();

public:
    static String socket_type_to_string(int socketType);
    static String connection_mode_to_string(int connectionMode);

    Thread* thread = nullptr;
    Mutex* mutex = nullptr;
    bool receive_with_bytes = false;
    Callable stringMessageHandler;
    Callable bytesMessageHandler;
#ifndef __EMSCRIPTEN__
    zmq::context_t context;
    zmq::socket_t socket;
#endif
    String addr;
    int socket_type;
    int connection_mode;
    String socket_filter;
    bool auto_start_receive_thread_after_send = false;
    bool need_to_start_receiving = false;

    bool isThreadRunning = false;

    ZMQSender();
    ~ZMQSender();

    static ZMQSender* new_from(String inAddr, int socketType, int connectionMode, String socketFilter, bool autoStartReceiveThreadAfterSend);
    void init(String inAddr, int socketType, int connectionMode, String socketFilter, bool autoStartReceiveThreadAfterSend);
    void _ready() override;
    void _process(double delta) override;
    void _thread_func();

    void stop();
    void sendString(String message);
    void sendBytes(PackedByteArray message);
    void onMessageString(Callable callback);
    void onMessageBytes(Callable callback);

    void beginReceiveRequest();
};
    
}
