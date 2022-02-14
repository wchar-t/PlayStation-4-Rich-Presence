import aiohttp
import copy
import json

class Activity:
    """
    Activity
    --------------
    ## Description
    Class representing a single activity. That's what Discord shows when you open a profile.
    Some attributes of related classes are omitted, so, if you're interested in what you can do,
    take a look:
    https://discord.com/developers/docs/topics/gateway#activity-object

    # TODO
    """

    def __init__(self, data):
        self.name = data.get("name", None)
        self.type = 0
        self.timestamps = {
            "start": data.get("start", None),
            "end": data.get("end", None)
        }
        self.application_id = data.get("application_id", 940876161955794945)
        self.details = data.get("details", None)
        self.state = data.get("state", None)
        self.assets = {
            "large_image": data.get("large_image", None),
            "large_text": data.get("large_text", None),
            "small_image": data.get("small_image", None),
            "small_text": data.get("small_text", None)
        }
        #self.assets = 


class Presence:
    """
    Presence
    --------------
    ## Description
    Class representing your presence, which includes 3 informations: afk, status and activities.
    Your activities are wrapped in here.

    :class:`List<Activity>`activities:
        The list of activities you are in. This project will only need one.
    :class:`str`status:
        Your status. It may be online, dnd, idle, invisible and offline.
    :class:`bool`afk:
        Away from keyboard state.
    """

    def __init__(self, activities, status="online", afk=False):
        self.since = None
        self.afk = afk
        self.status = status
        self.activities = activities

class DiscordGateway:
    """
    DiscordGateway
    --------------
    ## Description
    A simple connection to Discord's gateway. Just enough to maintain the presence part,
    with room to improve for the rest of the API.
    I suggest trying to expand this code, just for fun :)
    Reference:
    https://discord.com/developers/docs/topics/gateway
    https://discord.com/developers/docs/topics/opcodes-and-status-codes#gateway

    # TODO
    """

    def __init__(self):
        self.events = {}
        self.is_ready = False

    def event(self, coro):
        self.events[coro.__name__] = coro
        return coro

    async def dispatch(self, coro_name, *args, **kwargs):
        if coro_name in list(self.events.keys()):
            await self.events[coro_name](self.ws, *args, **kwargs)

    async def change_presence(self, presence):
        if not "ws" in dir(self):
            return
        
        _presence = copy.deepcopy(presence).__dict__
        _presence["activities"] = [x.__dict__ for x in _presence["activities"]]
        print(_presence)

        await self.ws.send_json({
            "op": 3,
            "d": _presence
        })

    async def identify(self):
        auth = {
            "token": self.token,
            "properties": {
                "os": "linux",
                "browser": "disco",
                "device": "disco"
            }
        }
        await self.ws.send_json({
            "op": 2,
            "d": auth
        })
        self.is_ready = True
        await self.dispatch("on_identify")

    async def _on_hello(self, data):
        # todo: data.heartbeat_interval
        await self.dispatch("on_hello", data)
        await self.identify()

    async def run(self, loop, token):
        self.loop = loop
        self.session = aiohttp.ClientSession()
        self.ws = await self.session.ws_connect("wss://gateway.discord.gg/?encoding=json&v=9")
        self.token = token

        while True:
            msg = await self.ws.receive()

            if msg.type == aiohttp.WSMsgType.TEXT:
                # as all msgs are json
                # todo: resume operation
                # todo: hello ack op create_task
                parsed_msg = json.loads(msg.data)
                op = parsed_msg.get("op", None)
                d = parsed_msg.get("d", {})

                if op == 10:
                    await self._on_hello(d)
                
            elif msg.type == aiohttp.WSMsgType.CLOSED:
                raise Exception("Discord closed its websocket connection.")

        await self.session.close()