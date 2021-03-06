#pragma once

extern "C"
{
    #include "lwip/api.h"
}

#include "netbuf.hpp"

namespace lwip
{
    class Netconn
    {
        struct netconn *conn;

    public:
        Netconn(struct netconn* conn) : conn(conn) {}

        operator netconn*() { return conn; }

        bool isAllocated() { return conn; }

        Netconn(enum netconn_type t, uint8_t proto = 0, netconn_callback callback = NULL)
        {
            conn = netconn_new_with_proto_and_callback(t, proto, callback);
        }

#ifdef ESP8266
        err_t bind(ip_addr_t* addr, u16_t port)
#else
        err_t bind(const ip_addr_t* addr, u16_t port)
#endif
        {
            return netconn_bind(conn, addr, port);
        }

        err_t listen(u8_t backlog = TCP_DEFAULT_LISTEN_BACKLOG)
        {
            return netconn_listen_with_backlog(conn, backlog);
        }

        err_t accept(struct netconn** new_conn)
        {
            return netconn_accept(conn, new_conn);
        }

        err_t disconnect()
        {
            return netconn_disconnect(conn);
        }

        err_t del()
        {
            return netconn_delete(conn);
        }

        err_t peer(ip_addr_t* addr, u16_t* port)
        {
            return netconn_peer(conn, addr, port);
        }

        err_t write(const void *dataptr, size_t size, u8_t apiflags, size_t *bytes_written = NULL)
        {
            return netconn_write_partly(conn, dataptr, size, apiflags, bytes_written);
        }

        err_t send(struct netbuf* buf)
        {
            return netconn_send(conn, buf);
        }

        err_t sendTo(struct netbuf* buf, ip_addr_t* addr, u16_t port)
        {
            return netconn_sendto(conn, buf, addr, port);
        }

        err_t sendTo(Netbuf& netbuf_from, Netbuf& netbuf_to)
        {
            return netconn_sendto(conn, netbuf_from,
                netbuf_to.fromAddr(), netbuf_to.fromPort());
        }

        err_t recv(struct netbuf **new_buf)
        {
            return netconn_recv(conn, new_buf);
        }

        err_t recv(Netbuf& netbuf)
        {
            return netconn_recv(conn, &netbuf.buf);
        }

        void set_recvtimeout(u16_t timeout)
        {
            netconn_set_recvtimeout(conn, timeout);
        }
    };


    /*
    class Netbuf
    {
        struct netbuf* buf;

    public:
        Netbuf() { buf = netbuf_new(); }
        ~Netbuf() { netbuf_delete(buf); }

        operator netbuf*()
        { return buf; }

        void* alloc(size_t size) { return netbuf_alloc(buf, size); }
    }; */
}
