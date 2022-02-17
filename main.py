import asyncio
from discord_gateway import DiscordGateway
from ps4_api import PS4Api

gate = DiscordGateway()
api = PS4Api(gate)


@gate.event
async def on_hello(ws, data):
    print("Hello, there :)")

async def run(loop, token):
    #loop.create_task(gate.run(loop, token))
    loop.create_task(api.run(loop))

    while True:
        # blocking operation
        await asyncio.sleep(1)

if __name__ == "__main__":
    with open("tokens/token.0.txt", "r") as f:
        token = f.read()
    
    loop = asyncio.get_event_loop()
    loop.run_until_complete(run(loop, token))
