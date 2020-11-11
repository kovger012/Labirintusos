import asyncio
import time

players = []
players_joined = []
is_ping = False


       
class Timer:
    def __init__(self, interval, first_immediately, timer_name, callback,trans):
        self._interval = interval
        self._first_immediately = first_immediately
        self._name = timer_name
        self._callback = callback
        self._is_first_call = True
        self._ok = True
        self._trans = trans
        self._task = asyncio.ensure_future(self._job())
        print(timer_name + " init done")

    async def _job(self):
        try:
            while self._ok:
                if not self._is_first_call or not self._first_immediately:
                    await asyncio.sleep(self._interval)
                await self._callback(self._name, self,trans)
                self._is_first_call = False
        except Exception as ex:
            print(ex)

    def cancel(self):
        self._ok = False
        self._task.cancel()


async def ping_callback(timer_name, timer, trans):
    for addr in players:
        msg_to_send = "HERE?" 
        sendto(msg_to_send.encode("utf-8"), addr)
    players_joined = []
    resp = Timer(3,False,"RespTimer",ping_responde_callbacktrans)

async def ping_responde_callback(timer_name, timer, trans):
    players = players_joined
    timer.cancel()

class EchoServerProtocol:
    def connection_made(self, transport):
        self.transport = transport

    def datagram_received(self, data, addr):
        message = data.decode()
        msg_to_send = ""
        print('Received %r from %s' % (message, addr))
        print('Send %r to %s' % (message, addr))

        if message == "HERE!":
                players_joined.append(addr)

        if message == "START":
            if len(players) < 4:
                msg_to_send = "OK"               
                players.append(addr)
                self.transport.sendto(msg_to_send.encode("utf-8"), addr)
            else:
                msg_to_send = "FULL"               
                self.transport.sendto(msg_to_send.encode("utf-8"), addr)
        self.transport.sendto(data, addr)


async def main():
    print("Starting UDP server")

    # Get a reference to the event loop as we plan to use
    # low-level APIs.
    loop = asyncio.get_running_loop()
    loop.run_forever()
    # One protocol instance will be created to serve all
    # client requests.
    transport, protocol = await loop.create_datagram_endpoint(
        lambda: EchoServerProtocol(),
        local_addr=('127.0.0.1', 9999))
    ping = Timer(10,False,"Ping",ping_callback)
    try:
        await asyncio.sleep(3600)  # Serve for 1 hour.
    finally:
        transport.close()


asyncio.run(main())