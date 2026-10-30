#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

namespace sl
{

    /**
     * Initialise the networking layer. Calls are reference counted and each must be paired with net_shutdown().
     * NetServer, NetClient, TcpSocket and TcpListener take their own reference while open, so calling this
     * yourself is only needed for code that wants to keep the layer alive between uses.
     */
    bool net_initialise();
    /** Release one reference taken by net_initialise(). */
    void net_shutdown();

    /** The kind of a NetEvent. */
    enum class NetEventType
    {
        /** A peer connected (server) or the connection was accepted (client). */
        connected,
        /** A peer disconnected, timed out or the connection attempt failed. */
        disconnected,
        /** A packet arrived. */
        received
    };

    /** One network event returned by NetServer::poll() or NetClient::poll(). */
    struct NetEvent
    {
        NetEventType type = NetEventType::received;
        /** Identifies the peer. Never 0, and unique among the peers of one server while they are connected. */
        std::uint32_t peer = 0;
        /** Channel a packet arrived on. */
        std::uint8_t channel = 0;
        /** The packet payload for received events. */
        std::vector<std::uint8_t> data;
    };

    /** A UDP game server with reliable and unreliable channels (built on ENet). */
    class NetServer
    {
    public:
        NetServer();
        ~NetServer();
        NetServer(const NetServer &) = delete;
        NetServer &operator=(const NetServer &) = delete;

        /** Start listening. Port 0 picks a free port; see port(). */
        bool start(std::uint16_t port, std::size_t max_clients = 32, std::size_t channels = 2);
        /** Disconnect every client and stop listening. */
        void stop();
        bool running() const;
        /** The port the server is bound to. */
        std::uint16_t port() const;
        /** Number of connected clients. */
        std::size_t client_count() const;
        /** Service the network and return the next event. Returns false when nothing is pending. Call it every frame. */
        bool poll(NetEvent &event);
        /** Send to one client. Reliable packets are ordered and retransmitted; unreliable ones may be dropped. */
        bool send(std::uint32_t peer, const void *data, std::size_t size, std::uint8_t channel = 0, bool reliable = true);
        bool send(std::uint32_t peer, const std::vector<std::uint8_t> &data, std::uint8_t channel = 0, bool reliable = true);
        /** Send to every connected client. */
        void broadcast(const void *data, std::size_t size, std::uint8_t channel = 0, bool reliable = true);
        void broadcast(const std::vector<std::uint8_t> &data, std::uint8_t channel = 0, bool reliable = true)
        {
            broadcast(data.data(), data.size(), channel, reliable);
        }
        /** Politely disconnect a client; a disconnected event follows. */
        void disconnect(std::uint32_t peer);

    private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };

    /** A UDP game client with reliable and unreliable channels (built on ENet). */
    class NetClient
    {
    public:
        NetClient();
        ~NetClient();
        NetClient(const NetClient &) = delete;
        NetClient &operator=(const NetClient &) = delete;

        /**
         * Begin connecting. The result arrives from poll() as a connected or disconnected event. The host name is
         * resolved before this returns, which can block briefly.
         */
        bool connect(const std::string &host, std::uint16_t port, std::size_t channels = 2);
        /** Tell the server we are leaving and close. No further events are produced. */
        void disconnect();
        /** True once the connection is established. */
        bool connected() const;
        /** True while connect() or an established connection is active. */
        bool active() const;
        /** Service the network and return the next event. Returns false when nothing is pending. Call it every frame. */
        bool poll(NetEvent &event);
        bool send(const void *data, std::size_t size, std::uint8_t channel = 0, bool reliable = true);
        bool send(const std::vector<std::uint8_t> &data, std::uint8_t channel = 0, bool reliable = true);
        /** Round trip time to the server in milliseconds, or 0 when not connected. */
        std::uint32_t ping_ms() const;

    private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };

    /** Builds a packet payload. Integers and floats are stored little-endian, so packets are portable. */
    class PacketWriter
    {
    public:
        template <typename Type>
        void write(Type value)
        {
            static_assert(std::is_arithmetic_v<Type> || std::is_enum_v<Type>, "PacketWriter::write needs a number");
            std::uint8_t bytes[sizeof(Type)];
            std::memcpy(bytes, &value, sizeof(Type));
            if (!little_endian())
                for (std::size_t i = 0; i < sizeof(Type) / 2; ++i)
                    std::swap(bytes[i], bytes[sizeof(Type) - 1 - i]);
            data_.insert(data_.end(), bytes, bytes + sizeof(Type));
        }
        /** Write a string with a 16-bit length prefix; longer strings are cut at 65535 bytes. */
        void write_string(std::string_view text);
        void write_bytes(const void *data, std::size_t size);
        const std::vector<std::uint8_t> &bytes() const { return data_; }
        const std::uint8_t *data() const { return data_.data(); }
        std::size_t size() const { return data_.size(); }
        void clear() { data_.clear(); }

    private:
        static bool little_endian()
        {
            const std::uint16_t one = 1;
            std::uint8_t first = 0;
            std::memcpy(&first, &one, 1);
            return first == 1;
        }
        std::vector<std::uint8_t> data_;
    };

    /** Reads a PacketWriter payload. Reads never overrun: a short packet makes the read fail and ok() false. */
    class PacketReader
    {
    public:
        PacketReader(const std::uint8_t *data, std::size_t size) : data_(data), size_(size) {}
        explicit PacketReader(const std::vector<std::uint8_t> &data) : data_(data.data()), size_(data.size()) {}

        template <typename Type>
        bool read(Type &value)
        {
            static_assert(std::is_arithmetic_v<Type> || std::is_enum_v<Type>, "PacketReader::read needs a number");
            std::uint8_t bytes[sizeof(Type)];
            if (!take(bytes, sizeof(Type)))
                return false;
            const std::uint16_t one = 1;
            std::uint8_t first = 0;
            std::memcpy(&first, &one, 1);
            if (first != 1)
                for (std::size_t i = 0; i < sizeof(Type) / 2; ++i)
                    std::swap(bytes[i], bytes[sizeof(Type) - 1 - i]);
            std::memcpy(&value, bytes, sizeof(Type));
            return true;
        }
        bool read_string(std::string &text);
        bool read_bytes(void *out, std::size_t size) { return take(out, size); }
        std::size_t remaining() const { return size_ - position_; }
        /** False once any read has failed. */
        bool ok() const { return ok_; }

    private:
        bool take(void *out, std::size_t size);
        const std::uint8_t *data_;
        std::size_t size_;
        std::size_t position_ = 0;
        bool ok_ = true;
    };

    /** Where a TcpSocket is in its life. */
    enum class TcpState
    {
        /** Not open: never connected, or closed. error() explains a failure. */
        closed,
        /** The host name is being resolved or the connection is in progress. */
        connecting,
        connected
    };

    /**
     * A non-blocking TCP stream, for talking to services such as IRC or HTTP. Nothing here blocks: connect() resolves
     * the host name on a worker thread, send() queues and receive() returns what has arrived. Call update() (or any
     * other method that does) every frame. There is no TLS, so the stream is plain text on the wire.
     */
    class TcpSocket
    {
    public:
        TcpSocket();
        ~TcpSocket();
        TcpSocket(TcpSocket &&other) noexcept;
        TcpSocket &operator=(TcpSocket &&other) noexcept;
        TcpSocket(const TcpSocket &) = delete;
        TcpSocket &operator=(const TcpSocket &) = delete;

        /** Start connecting to a host name or IP address. Any earlier connection is closed. */
        bool connect(const std::string &host, std::uint16_t port, int timeout_ms = 15000);
        /** Advance connecting, flush queued data and return the current state. */
        TcpState update();
        TcpState state() const;
        bool connected() const { return state() == TcpState::connected; }
        /**
         * Queue data to send and write as much as the socket accepts. Returns false if the socket is not open or the
         * unsent backlog exceeds 1 MiB.
         */
        bool send(std::string_view data);
        /** Append everything that has arrived to out and return how many bytes that was. A closed peer sets the state to closed. */
        std::size_t receive(std::string &out);
        /** Wait up to timeout_ms for data to read. For tools and tests; games should poll with receive(). */
        bool wait_readable(int timeout_ms);
        /** Close the socket and drop queued data. */
        void close();
        /** Why the socket is closed, or empty after an orderly close() call. */
        const std::string &error() const;

    private:
        friend class TcpListener;
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };

    /** Accepts incoming TCP connections. */
    class TcpListener
    {
    public:
        TcpListener();
        ~TcpListener();
        TcpListener(const TcpListener &) = delete;
        TcpListener &operator=(const TcpListener &) = delete;

        /** Listen on a port; 0 picks a free one (see port()). With loopback_only only programs on this machine can connect (127.0.0.1). */
        bool listen(std::uint16_t port, int backlog = 16, bool loopback_only = false);
        void close();
        bool listening() const;
        std::uint16_t port() const;
        /** Accept one waiting connection into a connected socket. Returns false when none is waiting. */
        bool accept(TcpSocket &out);

    private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };

} // namespace sl
