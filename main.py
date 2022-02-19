import asyncio
import socket
from discord_gateway import DiscordGateway
from ps4_api import PS4Api


PS4_IP = "192.168.100.70"
gate = DiscordGateway()
api = PS4Api(gate)


def connect():
    print("Trying to connect to the ps4...")
    while True:
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.connect((PS4_IP, 4747))
            break
        except:
            print("Couldn't connect. (1) Check PS4_IP. (2) Payload should be running")


@gate.event
async def on_hello(ws, data):
    print("Discord connected.")

async def run(loop, token):
    connect()
    loop.create_task(gate.run(loop, token))
    loop.create_task(api.run(loop))

    while True:
        # blocking operation
        await asyncio.sleep(1)

if __name__ == "__main__":
    with open("tokens/token.0.txt", "r") as f:
        token = f.read()
    
    loop = asyncio.get_event_loop()
    loop.run_until_complete(run(loop, token))
