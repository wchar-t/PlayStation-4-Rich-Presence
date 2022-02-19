import asyncio
from datetime import datetime
from discord_gateway import DiscordGateway, Presence, Activity
from aiohttp import web

class PS4Api():
    def __init__(self, discord_gate: DiscordGateway):
        self.web = web.Application()
        self.web.add_routes([
            web.get("/set_activity", self.set_activity)
        ])
        self.discord_gate = discord_gate
        self.playing = {
            "start_timestamp": 0,
            "last_timestamp": 0,
            "cusa": "",
            "image_key": None,
            "title": None,
            "is_idle": False
        }
        self.presence_placeholder = Presence([Activity({
            "name": "PS4",
            "state": "Idle",
            "large_image": "942818002963951667",
            "large_text": "PlayStation 4"
        })])

        
        with open("data/cusa.txt", "r") as arq:
            self.cusa = arq.read().split("\n")

    def _find_by_cusa(self, cusa_id):
        if len(cusa_id) < 4:
            return
        
        cusa_id = cusa_id.lower()

        for cusa in self.cusa:
            if cusa_id in cusa:
                parts = cusa.split(",")
                return (parts[-2], parts[-1]) # asset, game_name
        
        return False
    
    async def _tick(self):
        # this will run and call discord_gate.change_presence
        # It isn't ideal to await for /set_activity to call it, as it may not be called in time
        while True:
            use_default = False
            now_timestamp = int(datetime.now().timestamp())
            time_diff = now_timestamp - self.playing["last_timestamp"]
            presence = self.presence_placeholder
            cusa_data = self._find_by_cusa(self.playing["cusa"])

            if time_diff >= 10 or not cusa_data:
                # 10 seconds without any /set_activity call
                # or game not found in cusa.txt
                if not self.playing["is_idle"]:
                    self.playing["is_idle"] = True
                    self.playing["start_timestamp"] = now_timestamp
            else:
                self.playing["is_idle"] = False

                if not self.playing["title"] == cusa_data[1]:
                    self.playing["start_timestamp"] = now_timestamp
                    self.playing["title"] = cusa_data[1]

                presence = Presence([Activity({
                    "name": cusa_data[1],
                    "large_image": cusa_data[0],
                    "large_text": cusa_data[1],
                    "small_image": "942818002963951667",
                    "small_text": "Playing on PS4"
                })])

            presence.activities[0].timestamps = {"start": self.playing["start_timestamp"]}

            if self.discord_gate.is_ready:
                await self.discord_gate.change_presence(presence)
            await asyncio.sleep(1)

    async def set_activity(self, request):
        # ideally we will receive all apps running, and here we parse
        # it doesn't matter, as long as our cusa is in ?mounted=
        url = str(request.url).lower()
        
        if not "mounted" in request.query.keys():
            return web.json_response({"success": False})
        
        if not "cusa" in url:
            return web.json_response({"success": False})
        
        mounted_dirs = request.query.get("mounted").lower().split("|")
        self.playing["cusa"] = [x for x in mounted_dirs if x.find("cusa") >= 0][0].split("_")[0].lower()
        self.playing["last_timestamp"] = int(datetime.now().timestamp())
        
        return web.json_response({"success": True})
        
    async def run(self, loop):
        self.loop = loop
        self.loop.create_task(self._tick())
        runner = web.AppRunner(self.web)
        await runner.setup()
        await web.TCPSite(runner, host="0.0.0.0", port=4848).start()