#define ENET_IMPLEMENTATION
#include "third_party/enet.h"

#include "net.h"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>
#include <utility>

#include "error.h"

namespace sl
{
    namespace
    {
        std::mutex net_mutex;
        int net_references = 0;

        // Holds one net_initialise() reference for as long as it is owned.
        struct NetRef
        {
            bool held = false;
            bool acquire()
            {
                if (!held)
                    held = net_initialise();
                return held;
            }
            void release()
            {
                if (held)
                    net_shutdown();
                held = false;
            }
            ~NetRef() { release(); }
        };

        bool resolve_numeric(const std::string &host, ENetAddress &out)
        {
            std::memset(&out, 0, sizeof(out));
#ifdef ENET_IPV4_ONLY
            return enet_address_set_host_ip_old(&out, host.c_str()) == 0;
#else
            return enet_address_set_host_ip_new(&out, host.c_str()) == 0;
#endif
        }

        bool resolve_name(const std::string &host, ENetAddress &out)
        {
            std::memset(&out, 0, sizeof(out));
#ifdef ENET_IPV4_ONLY
            return enet_address_set_host_old(&out, host.c_str()) == 0;
#else
            return enet_address_set_host_new(&out, host.c_str()) == 0;
#endif
        }

        ENetPacket *make_packet(const void *data, std::size_t size, bool reliable)
        {
            if (!data && size > 0)
                return nullptr;
            return enet_packet_create(data, size, reliable ? ENET_PACKET_FLAG_RELIABLE : 0);
        }

        void fill_event(NetEvent &event, const ENetEvent &enet_event, std::uint32_t peer)
        {
            event.peer = peer;
            event.channel = 0;
            event.data.clear();
            switch (enet_event.type)
            {
            case ENET_EVENT_TYPE_CONNECT:
                event.type = NetEventType::connected;
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
            case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
                event.type = NetEventType::disconnected;
                break;
            default:
                event.type = NetEventType::received;
                event.channel = enet_event.channelID;
                if (enet_event.packet)
                {
                    const auto *bytes = static_cast<const std::uint8_t *>(enet_event.packet->data);
                    event.data.assign(bytes, bytes + enet_event.packet->dataLength);
                }
                break;
            }
        }

        std::uint16_t bound_port(ENetSocket socket)
        {
            ENetAddress address;
            std::memset(&address, 0, sizeof(address));
            return enet_socket_get_address(socket, &address) == 0 ? address.port : 0;
        }
    } // namespace

    bool net_initialise()
    {
        std::lock_guard<std::mutex> lock(net_mutex);
        if (net_references == 0 && enet_initialize() != 0)
        {
            detail::set_error("Unable to initialise the networking layer.");
            return false;
        }
        ++net_references;
        return true;
    }

    void net_shutdown()
    {
        std::lock_guard<std::mutex> lock(net_mutex);
        if (net_references == 0)
            return;
        if (--net_references == 0)
            enet_deinitialize();
    }

    // ----- NetServer ----------------------------------------------------------------------------------------------

    struct NetServer::Impl
    {
        NetRef ref;
        ENetHost *host = nullptr;
        std::uint16_t port = 0;
        std::size_t clients = 0;

        ENetPeer *find(std::uint32_t id) const
        {
            if (!host || id == 0 || id > host->peerCount)
                return nullptr;
            ENetPeer *peer = &host->peers[id - 1];
            return peer->state == ENET_PEER_STATE_CONNECTED ? peer : nullptr;
        }
    };

    NetServer::NetServer() : impl_(std::make_unique<Impl>()) {}
    NetServer::~NetServer() { stop(); }

    bool NetServer::start(std::uint16_t port, std::size_t max_clients, std::size_t channels)
    {
        stop();
        if (max_clients == 0 || channels == 0 || !impl_->ref.acquire())
            return false;
        ENetAddress address;
        std::memset(&address, 0, sizeof(address));
        address.host = ENET_HOST_ANY;
        address.port = port;
        impl_->host = enet_host_create(&address, max_clients, channels, 0, 0);
        if (!impl_->host)
        {
            detail::set_error("Unable to listen on UDP port " + std::to_string(port) + ".");
            impl_->ref.release();
            return false;
        }
        impl_->port = bound_port(impl_->host->socket);
        if (impl_->port == 0)
            impl_->port = port;
        return true;
    }

    void NetServer::stop()
    {
        if (impl_->host)
        {
            for (std::size_t i = 0; i < impl_->host->peerCount; ++i)
                if (impl_->host->peers[i].state == ENET_PEER_STATE_CONNECTED)
                    enet_peer_disconnect_now(&impl_->host->peers[i], 0);
            enet_host_flush(impl_->host);
            enet_host_destroy(impl_->host);
            impl_->host = nullptr;
        }
        impl_->clients = 0;
        impl_->port = 0;
        impl_->ref.release();
    }

    bool NetServer::running() const { return impl_->host != nullptr; }
    std::uint16_t NetServer::port() const { return impl_->port; }
    std::size_t NetServer::client_count() const { return impl_->clients; }

    bool NetServer::poll(NetEvent &event)
    {
        if (!impl_->host)
            return false;
        ENetEvent enet_event;
        if (enet_host_service(impl_->host, &enet_event, 0) <= 0 || enet_event.type == ENET_EVENT_TYPE_NONE)
            return false;
        const std::uint32_t id = static_cast<std::uint32_t>(enet_event.peer - impl_->host->peers) + 1;
        fill_event(event, enet_event, id);
        if (enet_event.type == ENET_EVENT_TYPE_CONNECT)
            ++impl_->clients;
        else if (event.type == NetEventType::disconnected && impl_->clients > 0)
            --impl_->clients;
        if (enet_event.packet)
            enet_packet_destroy(enet_event.packet);
        return true;
    }

    bool NetServer::send(std::uint32_t peer, const void *data, std::size_t size, std::uint8_t channel, bool reliable)
    {
        ENetPeer *target = impl_->find(peer);
        if (!target || channel >= target->channelCount)
            return false;
        ENetPacket *packet = make_packet(data, size, reliable);
        if (!packet)
            return false;
        if (enet_peer_send(target, channel, packet) != 0)
        {
            enet_packet_destroy(packet);
            return false;
        }
        return true;
    }

    bool NetServer::send(std::uint32_t peer, const std::vector<std::uint8_t> &data, std::uint8_t channel, bool reliable)
    {
        return send(peer, data.data(), data.size(), channel, reliable);
    }

    void NetServer::broadcast(const void *data, std::size_t size, std::uint8_t channel, bool reliable)
    {
        if (!impl_->host || impl_->clients == 0 || channel >= impl_->host->channelLimit)
            return;
        // Packets are per peer so a peer with fewer channels does not reject the whole broadcast.
        for (std::size_t i = 0; i < impl_->host->peerCount; ++i)
            send(static_cast<std::uint32_t>(i + 1), data, size, channel, reliable);
    }

    void NetServer::disconnect(std::uint32_t peer)
    {
        if (ENetPeer *target = impl_->find(peer))
            enet_peer_disconnect(target, 0);
    }

    // ----- NetClient ----------------------------------------------------------------------------------------------

    struct NetClient::Impl
    {
        NetRef ref;
        ENetHost *host = nullptr;
        ENetPeer *peer = nullptr;
        bool connected = false;
    };

    NetClient::NetClient() : impl_(std::make_unique<Impl>()) {}
    NetClient::~NetClient() { disconnect(); }

    bool NetClient::connect(const std::string &host, std::uint16_t port, std::size_t channels)
    {
        disconnect();
        if (channels == 0 || !impl_->ref.acquire())
            return false;
        ENetAddress address;
        if (!resolve_name(host, address))
        {
            detail::set_error("Unable to resolve host " + host + ".");
            impl_->ref.release();
            return false;
        }
        address.port = port;
        impl_->host = enet_host_create(nullptr, 1, channels, 0, 0);
        if (impl_->host)
            impl_->peer = enet_host_connect(impl_->host, &address, channels, 0);
        if (!impl_->peer)
        {
            detail::set_error("Unable to start a connection to " + host + ".");
            disconnect();
            return false;
        }
        return true;
    }

    void NetClient::disconnect()
    {
        if (impl_->host)
        {
            if (impl_->peer && impl_->connected)
                enet_peer_disconnect_now(impl_->peer, 0);
            enet_host_flush(impl_->host);
            enet_host_destroy(impl_->host);
        }
        impl_->host = nullptr;
        impl_->peer = nullptr;
        impl_->connected = false;
        impl_->ref.release();
    }

    bool NetClient::connected() const { return impl_->connected; }
    bool NetClient::active() const { return impl_->peer != nullptr; }

    bool NetClient::poll(NetEvent &event)
    {
        if (!impl_->host)
            return false;
        ENetEvent enet_event;
        if (enet_host_service(impl_->host, &enet_event, 0) <= 0 || enet_event.type == ENET_EVENT_TYPE_NONE)
            return false;
        fill_event(event, enet_event, 1);
        if (enet_event.type == ENET_EVENT_TYPE_CONNECT)
            impl_->connected = true;
        else if (event.type == NetEventType::disconnected)
        {
            impl_->connected = false;
            impl_->peer = nullptr;
        }
        if (enet_event.packet)
            enet_packet_destroy(enet_event.packet);
        return true;
    }

    bool NetClient::send(const void *data, std::size_t size, std::uint8_t channel, bool reliable)
    {
        if (!impl_->peer || !impl_->connected || channel >= impl_->peer->channelCount)
            return false;
        ENetPacket *packet = make_packet(data, size, reliable);
        if (!packet)
            return false;
        if (enet_peer_send(impl_->peer, channel, packet) != 0)
        {
            enet_packet_destroy(packet);
            return false;
        }
        return true;
    }

    bool NetClient::send(const std::vector<std::uint8_t> &data, std::uint8_t channel, bool reliable)
    {
        return send(data.data(), data.size(), channel, reliable);
    }

    std::uint32_t NetClient::ping_ms() const
    {
        return impl_->peer && impl_->connected ? impl_->peer->roundTripTime : 0;
    }

    // ----- Packets ------------------------------------------------------------------------------------------------

    void PacketWriter::write_string(std::string_view text)
    {
        const std::size_t length = std::min<std::size_t>(text.size(), 0xFFFF);
        write(static_cast<std::uint16_t>(length));
        write_bytes(text.data(), length);
    }

    void PacketWriter::write_bytes(const void *data, std::size_t size)
    {
        const auto *bytes = static_cast<const std::uint8_t *>(data);
        if (bytes && size > 0)
            data_.insert(data_.end(), bytes, bytes + size);
    }

    bool PacketReader::take(void *out, std::size_t size)
    {
        if (!ok_ || size > size_ - position_)
        {
            ok_ = false;
            return false;
        }
        if (size > 0)
            std::memcpy(out, data_ + position_, size);
        position_ += size;
        return true;
    }

    bool PacketReader::read_string(std::string &text)
    {
        std::uint16_t length = 0;
        if (!read(length) || length > remaining())
        {
            ok_ = false;
            return false;
        }
        text.assign(reinterpret_cast<const char *>(data_ + position_), length);
        position_ += length;
        return true;
    }

    // ----- TcpSocket ----------------------------------------------------------------------------------------------

    namespace
    {
        // Host name lookup runs off the calling thread because getaddrinfo can stall for many seconds.
        struct Resolution
        {
            NetRef ref;
            std::atomic<bool> done{false};
            bool ok = false;
            ENetAddress address;
        };

        constexpr std::size_t max_backlog = 1 << 20;
    } // namespace

    struct TcpSocket::Impl
    {
        NetRef ref;
        ENetSocket socket = ENET_SOCKET_NULL;
        TcpState state = TcpState::closed;
        std::string error;
        std::string pending;
        std::size_t pending_offset = 0;
        std::shared_ptr<Resolution> resolution;
        std::uint16_t port = 0;
        std::string host;
        std::chrono::steady_clock::time_point deadline;

        void reset()
        {
            if (socket != ENET_SOCKET_NULL)
                enet_socket_destroy(socket);
            socket = ENET_SOCKET_NULL;
            resolution.reset();
            pending.clear();
            pending_offset = 0;
            state = TcpState::closed;
            ref.release();
        }

        void fail(const std::string &message)
        {
            reset();
            error = message;
        }

        void configure()
        {
#ifndef ENET_IPV4_ONLY
            enet_socket_set_option(socket, ENET_SOCKOPT_IPV6_V6ONLY, 0);
#endif
            enet_socket_set_option(socket, ENET_SOCKOPT_NONBLOCK, 1);
            enet_socket_set_option(socket, ENET_SOCKOPT_NODELAY, 1);
        }

        void begin_connect(const ENetAddress &address)
        {
            socket = enet_socket_create(ENET_SOCKET_TYPE_STREAM);
            if (socket == ENET_SOCKET_NULL)
            {
                fail("Unable to create a TCP socket.");
                return;
            }
            configure();
            if (enet_socket_connect(socket, &address) < 0)
                fail("Unable to connect to " + host + ":" + std::to_string(port) + ".");
        }

        void flush()
        {
            while (state == TcpState::connected && pending_offset < pending.size())
            {
                ENetBuffer buffer;
                buffer.data = const_cast<char *>(pending.data()) + pending_offset;
                buffer.dataLength = pending.size() - pending_offset;
                const int sent = enet_socket_send(socket, nullptr, &buffer, 1);
                if (sent > 0)
                    pending_offset += static_cast<std::size_t>(sent);
                else if (sent == 0 || sent == -2)
                    return;
                else
                {
                    fail("Connection lost while sending.");
                    return;
                }
            }
            if (pending_offset >= pending.size())
            {
                pending.clear();
                pending_offset = 0;
            }
        }

        bool readable(int timeout_ms)
        {
            enet_uint32 condition = ENET_SOCKET_WAIT_RECEIVE;
            return enet_socket_wait(socket, &condition, static_cast<enet_uint64>(timeout_ms)) == 0 &&
                   (condition & ENET_SOCKET_WAIT_RECEIVE);
        }
    };

    TcpSocket::TcpSocket() : impl_(std::make_unique<Impl>()) {}
    TcpSocket::~TcpSocket() { close(); }
    TcpSocket::TcpSocket(TcpSocket &&other) noexcept : impl_(std::move(other.impl_))
    {
        other.impl_ = std::make_unique<Impl>();
    }

    TcpSocket &TcpSocket::operator=(TcpSocket &&other) noexcept
    {
        if (this != &other)
        {
            close();
            impl_ = std::move(other.impl_);
            other.impl_ = std::make_unique<Impl>();
        }
        return *this;
    }

    bool TcpSocket::connect(const std::string &host, std::uint16_t port, int timeout_ms)
    {
        close();
        impl_->error.clear();
        if (host.empty() || port == 0)
        {
            impl_->error = "A host and port are required.";
            return false;
        }
        if (!impl_->ref.acquire())
        {
            impl_->error = last_error();
            return false;
        }
        impl_->host = host;
        impl_->port = port;
        impl_->state = TcpState::connecting;
        impl_->deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout_ms);

        ENetAddress address;
        if (resolve_numeric(host, address))
        {
            address.port = port;
            impl_->begin_connect(address);
            return impl_->state == TcpState::connecting;
        }
        auto resolution = std::make_shared<Resolution>();
        if (!resolution->ref.acquire())
        {
            impl_->fail(last_error());
            return false;
        }
        impl_->resolution = resolution;
        std::thread(
            [resolution, host]
            {
                resolution->ok = resolve_name(host, resolution->address);
                resolution->done = true;
            })
            .detach();
        return true;
    }

    TcpState TcpSocket::update()
    {
        Impl &state = *impl_;
        if (state.state == TcpState::connecting)
        {
            if (state.resolution)
            {
                if (state.resolution->done)
                {
                    const std::shared_ptr<Resolution> resolution = std::move(state.resolution);
                    state.resolution.reset();
                    if (!resolution->ok)
                    {
                        state.fail("Unable to resolve host " + state.host + ".");
                        return state.state;
                    }
                    resolution->address.port = state.port;
                    state.begin_connect(resolution->address);
                }
            }
            else if (state.socket != ENET_SOCKET_NULL)
            {
                enet_uint32 condition = ENET_SOCKET_WAIT_SEND;
                if (enet_socket_wait(state.socket, &condition, 0) == 0 && (condition & ENET_SOCKET_WAIT_SEND))
                {
                    int error = 0;
                    if (enet_socket_get_option(state.socket, ENET_SOCKOPT_ERROR, &error) != 0 || error != 0)
                        state.fail("Connection to " + state.host + ":" + std::to_string(state.port) + " failed.");
                    else
                        state.state = TcpState::connected;
                }
            }
            if (state.state == TcpState::connecting && std::chrono::steady_clock::now() > state.deadline)
                state.fail("Connection to " + state.host + ":" + std::to_string(state.port) + " timed out.");
        }
        state.flush();
        return state.state;
    }

    TcpState TcpSocket::state() const { return impl_->state; }

    bool TcpSocket::send(std::string_view data)
    {
        Impl &state = *impl_;
        if (state.state == TcpState::closed)
            return false;
        if (state.pending.size() - state.pending_offset + data.size() > max_backlog)
            return false;
        state.pending.append(data.data(), data.size());
        state.flush();
        return state.state != TcpState::closed;
    }

    std::size_t TcpSocket::receive(std::string &out)
    {
        Impl &state = *impl_;
        if (update() != TcpState::connected)
            return 0;
        std::size_t total = 0;
        char chunk[4096];
        for (int round = 0; round < 64 && state.state == TcpState::connected && state.readable(0); ++round)
        {
            ENetBuffer buffer;
            buffer.data = chunk;
            buffer.dataLength = sizeof(chunk);
            const int received = enet_socket_receive(state.socket, nullptr, &buffer, 1);
            if (received > 0)
            {
                out.append(chunk, static_cast<std::size_t>(received));
                total += static_cast<std::size_t>(received);
            }
            else if (received == 0)
                state.fail("Connection closed by the remote host.");
            else
                state.fail("Connection lost while receiving.");
        }
        return total;
    }

    bool TcpSocket::wait_readable(int timeout_ms)
    {
        return impl_->state == TcpState::connected && impl_->readable((std::max)(timeout_ms, 0));
    }

    void TcpSocket::close()
    {
        impl_->reset();
        impl_->error.clear();
    }

    const std::string &TcpSocket::error() const { return impl_->error; }

    // ----- TcpListener --------------------------------------------------------------------------------------------

    struct TcpListener::Impl
    {
        NetRef ref;
        ENetSocket socket = ENET_SOCKET_NULL;
        std::uint16_t port = 0;
    };

    TcpListener::TcpListener() : impl_(std::make_unique<Impl>()) {}
    TcpListener::~TcpListener() { close(); }

    bool TcpListener::listen(std::uint16_t port, int backlog, bool loopback_only)
    {
        close();
        if (!impl_->ref.acquire())
            return false;
        ENetSocket socket = enet_socket_create(ENET_SOCKET_TYPE_STREAM);
        ENetAddress address;
        std::memset(&address, 0, sizeof(address));
        address.host = loopback_only ? enet_v4_localhost : ENET_HOST_ANY;
        address.port = port;
        if (socket != ENET_SOCKET_NULL)
        {
#ifndef ENET_IPV4_ONLY
            enet_socket_set_option(socket, ENET_SOCKOPT_IPV6_V6ONLY, 0);
#endif
            enet_socket_set_option(socket, ENET_SOCKOPT_REUSEADDR, 1);
            enet_socket_set_option(socket, ENET_SOCKOPT_NONBLOCK, 1);
        }
        if (socket == ENET_SOCKET_NULL || enet_socket_bind(socket, &address) < 0 || enet_socket_listen(socket, backlog) < 0)
        {
            if (socket != ENET_SOCKET_NULL)
                enet_socket_destroy(socket);
            detail::set_error("Unable to listen on TCP port " + std::to_string(port) + ".");
            impl_->ref.release();
            return false;
        }
        impl_->socket = socket;
        impl_->port = bound_port(socket);
        if (impl_->port == 0)
            impl_->port = port;
        return true;
    }

    void TcpListener::close()
    {
        if (impl_->socket != ENET_SOCKET_NULL)
            enet_socket_destroy(impl_->socket);
        impl_->socket = ENET_SOCKET_NULL;
        impl_->port = 0;
        impl_->ref.release();
    }

    bool TcpListener::listening() const { return impl_->socket != ENET_SOCKET_NULL; }
    std::uint16_t TcpListener::port() const { return impl_->port; }

    bool TcpListener::accept(TcpSocket &out)
    {
        if (impl_->socket == ENET_SOCKET_NULL)
            return false;
        enet_uint32 condition = ENET_SOCKET_WAIT_RECEIVE;
        if (enet_socket_wait(impl_->socket, &condition, 0) != 0 || !(condition & ENET_SOCKET_WAIT_RECEIVE))
            return false;
        ENetSocket accepted = enet_socket_accept(impl_->socket, nullptr);
        if (accepted == ENET_SOCKET_NULL)
            return false;
        out.close();
        if (!out.impl_->ref.acquire())
        {
            enet_socket_destroy(accepted);
            return false;
        }
        out.impl_->socket = accepted;
        out.impl_->configure();
        out.impl_->state = TcpState::connected;
        return true;
    }

} // namespace sl
